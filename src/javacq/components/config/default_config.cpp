//
// Created by Sobte on 2019-12-05.
//
#include <filesystem>
#include <fstream>

#include "default_config.h"

#include "javacq/utils/env.h"

using namespace std;

namespace fs = std::filesystem;

namespace jcq::components {

    static const auto DEFAULT_CONFIG = string() + R"({

    })";

    static const auto TAG = u8"配置";

    void DefaultConfig::hook_start(Context &ctx) {
        if (!ctx.config->get_bool("_LOADED", false)) {
            // there is no config file now, we should generate a default one

            const auto login_id_str = to_string(cq::get_login_user_id());
            const auto filename = ansi(utils::dir::conf() + "JCQ.cfg");
            remove(filename.data()); // may be error config, remove file
            if (ofstream file(filename); file.is_open()) {
                file << "[JCQ]" << endl;
                file << "jre.path=" << endl;
                file << "jre.path=" << endl;
                file.close();
            } else {
                logging::error(TAG, u8"配置写入失败，请检查文件系统权限");
            }

            try {
                ctx.config->raw = json::parse(DEFAULT_CONFIG);
            } catch (...) {
            }
        }
        // jre path empty
        if (ctx.config->get_string("jre.path").empty()) {
            string java_env_path = "SOFTWARE\\JavaSoft\\Java Runtime Environment";
            string current_version = utils::env::get_registry_val(java_env_path, "CurrentVersion");
            if (!current_version.empty()) {
                string java_home = utils::env::get_registry_val(java_env_path + "\\" + current_version, "JavaHome");
                ctx.config->put("jre.path", java_home);
            }
        }

        // java home env
        if (ctx.config->get_string("jre.path").empty()) {
            string java_home = utils::env::get_env_variable("JAVA_HOME");
            if (!java_home.empty()
                && (fs::exists(fs::path(java_home) / "jre" / "server" / "jvm.dll")
                    || fs::exists(fs::path(java_home) / "jre" / "client" / "jvm.dll"))) {
                ctx.config->put("jre.path", java_home);
            }
        }

        // TODO convert unicode emoji

        // main class
        const auto default_main_class = "org.meowy.cqp.JCQManager";
        if (ctx.config->get_bool("custom.enable")) {
            logging::info_recv(
                TAG, u8"请注意！自定义已开启，可更改JCQ核心配置信息\n此模式下如遇到严重BUG，建议使用JCQ自带主类！！！");
            ctx.config->put("main-class", ctx.config->get_string("custom.main-class", default_main_class));
        } else {
            ctx.config->put("main-class", default_main_class);
        }

        ctx.config->raw.erase("_LOADED");

        ctx.next();
    }
} // namespace jcq::components