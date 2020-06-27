//
// Created by Sobte on 2019/12/8.
//
#include "http.h"

#include <curl/curl.h>
#include <filesystem>
#include <fstream>
#include <regex>

#include <algorithm>
#include <boost/algorithm/string.hpp>

#include "javacq/core/exception.h"

using namespace std;
namespace fs = std::filesystem;

#define DEFAULT_USER_AGENT                       \
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) " \
    "AppleWebKit/537.36 (KHTML, like Gecko) "    \
    "Chrome/79.0.3945.79 Safari/537.36"

namespace jcq::utils::http {


    inline static void __throw_if_needed(const void *const ret_ptr) noexcept(false) {
        if (!ret_ptr) {
            throw exception::RuntimeError("INVALID DATA");
        }
    }

    Response HttpClient::execute(const Request &request) {

        // curl init begin
        const auto curl = curl_easy_init();
        __throw_if_needed(curl);
        // curl init end

        // request begin
        curl_easy_setopt(curl, CURLOPT_URL, request.url.c_str());// url

        if (request.method == Method::POST) {// method
            curl_easy_setopt(curl, CURLOPT_POST, 1);// post
        }

        const auto request_body = request.body;// body
        Headers request_headers = request.headers;// headers

        if (!request_body.content_type.empty()) {// body type
            request_headers["Content-Type"] = request_body.content_type;
        }

        if (!request_body.content_body.empty()) {// body content
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_body.content_body.c_str());
        }

        struct curl_slist *chunk = nullptr;// header
        for (const auto &header : request_headers) {
            chunk = curl_slist_append(chunk, (header.first + ": " + header.second).c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        // request end

        // response begin
        Headers response_headers;
        ResponseBody response_body;

        const auto header_callback = [](char *buf, size_t size, size_t count,
                                        void *headers) {// response header callback
            auto line = string(buf, count);
            const auto sep_pos = line.find(':');
            if (sep_pos != string::npos) {
                const auto k = line.substr(0, sep_pos);
                auto v = line.substr(sep_pos + 1);
                boost::algorithm::trim_left(v);
                (*static_cast<Headers *>(headers))[k] = v;
            }
            return size * count;
        };
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &response_headers);// response headers
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, static_cast<CallbackFunction>(header_callback));

        if (!body_callback) {// response body callback
            body_callback = [](char *buf, size_t size, size_t count,
                               void *body) {
                *static_cast<string *>(body) += string(buf, count);
                return size * count;
            };
            body_data = &response_body.content_body;
        }
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body_data);// response body
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, static_cast<CallbackFunction>(body_callback));
        // response end

        // timeout begin
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, request.connect_timeout);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, request.read_timeout);
        // timeout end

        // result begin
        int curl_code = curl_easy_perform(curl);
        int code = 0;
        if (curl_code == CURLE_OK) {
            long response_code;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            code = static_cast<int>(response_code);
            if (response_headers.find("Content-Type") != response_headers.end()) {
                response_body.content_type = response_headers["Content-Type"];
            }
            if (response_headers.find("Content-Length") != response_headers.end()) {
                response_body.content_length = size_t(stoll(response_headers["Content-Length"]));
            }
        }
        // result end

        // clean resources begin
        curl_slist_free_all(chunk);
        curl_easy_cleanup(curl);
        // clean resources end

        return Response(request, code, response_headers, response_body);
    }

    static void fix_headers(Headers &headers) {
        if (headers.find("User-Agent") == headers.end()) {
            headers["User-Agent"] = DEFAULT_USER_AGENT;
        }
    }

    Response get(const Request &request) {
        auto http_client = HttpClient();
        return http_client.execute(request);
    }

    Response get(const string &url, Headers headers, long timeout) {
        fix_headers(headers);
        auto request = Request(url, headers);
        request.read_timeout = timeout;
        return get(request);
    }

    bool download(const string &url, const string &path, const bool use_jcq_ua) {
        auto result = false;
        const auto request = Request(url, {{"User-Agent", use_jcq_ua ? JCQ_USER_AGENT : DEFAULT_USER_AGENT}});
        const auto ansi_path = ansi(path);

        auto client = HttpClient();

        struct {
            size_t count;
            ofstream file;
        } write_data_wrapper{0, ofstream(ansi_path, ios::out | ios::binary)};

        if (write_data_wrapper.file.is_open()) {
            client.body_data = &write_data_wrapper;
            client.body_callback = [](char *buf, size_t size, size_t count, void *data) -> size_t {
                auto wrapper = static_cast<decltype(write_data_wrapper) *>(data);
                wrapper->file.write(buf, count);
                size_t current_count = size * count;
                wrapper->count += current_count;
                return current_count;
            };

            const auto response = client.execute(request);
            write_data_wrapper.file.close();

            if (response.code >= 200 && response.code < 300
                && ((response.body.content_length > 0 && write_data_wrapper.count == response.body.content_length)
                    || (response.body.content_length == 0 && write_data_wrapper.count > 0))) {
                logging::debug(u8"下载文件",
                               path + "\ndownloaded size: " + to_string(write_data_wrapper.count)
                               + "\ncontent length: " + to_string(response.body.content_length)
                               + "\nfile size: " + to_string(fs::file_size(ansi_path)));
                result = true;
            }
        }

        if (!result && fs::exists(ansi_path)) {
            fs::remove(ansi_path);
        }

        return result;
    }

    optional<json> get_json(const string &url, const bool use_jcq_ua, const string &cookies) {
        Headers headers = {
                {"User-Agent", use_jcq_ua ? JCQ_USER_AGENT : DEFAULT_USER_AGENT},
                {"Referer",    url}
        };

        if (!cookies.empty()) {
            headers["Cookie"] = cookies;
        }

        const auto request = Request(url, headers);

        if (const auto response = get(request); response.code >= 200 && response.code < 300) {
            auto body = response.body.content_body;
            if (smatch m; regex_search(body, m, regex("\\);?\\s*$"))) {
                // is jsonp
                if (const auto start = body.find('('); start != string::npos) {
                    body = body.substr(start + 1, body.size() - (start + 1) - m.length());
                }
            }
            if (!body.empty()) {
                try {
                    return json::parse(body);
                } catch (json::parse_error &) {
                }
            }
        }

        return nullopt;
    }


} // namespace jcq::utils::http