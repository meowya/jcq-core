//
// Created by Sobte on 2019/12/15.
//

#include "helper.h"

#include "javacq/core/core.h"

#include "http.h"

using namespace std;

namespace jcq::utils::helper {

    static const vector<string> resources_list = {
            "JCQ-Manager.jar",
            "JCQ-CoolQ.jar"
    };

    string get_git_raw_url(const string &author, const string &project, const string &path, const string &type) {
        auto url = string();
        if (type == "coding") {
            url = "https://coding.net/u/" + author + "/p/" + project + "/git/raw/master/";
        } else if (type == "gitee") {
            url = "https://gitee.com/" + author + "/" + project + "/raw/master/";
        } else if (type == "github" || url.empty()) {
            url = "https://raw.githubusercontent.com/" + author + "/" + project + "/master/";
        }
        return url + path;
    }

    string get_source_url(const string &path) {
        string type = app.config().get_string("source.type", "coding");
        string author = "Meowy";
        if (type == "github") {
            author += "a";// Meowya
        }
        return get_git_raw_url(author, "JCQ-Core-Releases", path, type);
    }

    vector<string> get_resources_list(const string &version, const int build_number) {
        const auto json_opt = http::get_json(get_version_info_url(version, build_number));
        if (!json_opt) {
            return resources_list;
        }

        auto json = json_opt.value();
        if (json.is_object() && json.find("resources") != json.end() && json["resources"].is_array()) {
            return json["resources"].get<vector<string>>();
        }

        return resources_list;
    }


} // namespace jcq::utils::helper