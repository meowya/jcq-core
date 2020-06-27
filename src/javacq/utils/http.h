//
// Created by Sobte on 2019/12/8.
//

#include "javacq/core/common.h"

#include <map>

namespace jcq::utils::http {

    enum class Method {
        GET, POST
    };

    struct CaseInsensitiveCompare {
        bool operator()(const std::string &a, const std::string &b) const noexcept {
            return _stricmp(a.c_str(), b.c_str()) < 0;
        }
    };

    using Headers = std::map<std::string, std::string, CaseInsensitiveCompare>;

    struct HttpBody {
        std::string content_type;
        std::string content_body;
        size_t content_length = 0;

        HttpBody() = default;

        HttpBody(const std::string &content_type, const size_t &content_length, const std::string &content_body)
                : content_type(content_type), content_length(content_length), content_body(content_body) {}
    };

    struct RequestBody : HttpBody {
        RequestBody() : HttpBody() {}

        RequestBody(const std::string &content_type, const std::string &content_body)
                : HttpBody(content_type, -1, content_body) {}
    };

    struct ResponseBody : HttpBody {
        ResponseBody() : HttpBody() {}

        ResponseBody(const std::string &content_type, const size_t &content_length, const std::string &content_body)
                : HttpBody(content_type, content_length, content_body) {}
    };

    struct Request {
        const std::string url;
        Method method = Method::GET;
        Headers headers;
        RequestBody body;
        long connect_timeout = 0;
        long read_timeout = 0;

        Request() = default;

        Request(const std::string &url) : url(url) {}

        Request(const std::string &url, const Headers &headers) : url(url), headers(headers) {}

        Request(const std::string &url, const RequestBody &body) : url(url), body(body) {}

        Request(const std::string &url, const Headers &headers, const RequestBody &body) :
                url(url), headers(headers), body(body) {}
    };

    struct Response {
        const Request request;
        const int code = 0;
        const Headers headers;
        const ResponseBody body;

        Response(const Request &request, const int &code, const Headers &headers, const ResponseBody &body)
                : request(request), code(code), headers(headers), body(body) {}
    };

    typedef size_t (*CallbackFunction)(char *, size_t, size_t, void *);

    struct HttpClient {
        void *body_data = nullptr;

        CallbackFunction body_callback = nullptr;

        Response execute(const Request &request);
    };

    Response get(const Request &request);

    Response get(const std::string &url, Headers headers = {}, long timeout = 0);

    bool download(const std::string &url, const std::string &path, bool use_jcq_ua = false);

    std::optional<json> get_json(const std::string &url, bool use_jcq_ua = false, const std::string &cookies = "");

} // namespace jcq::utils::http
