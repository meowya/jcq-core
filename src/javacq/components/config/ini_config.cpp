//
// Created by Sobte on 2019-12-06.
//

#include <boost/property_tree/ini_parser.hpp>
#include <filesystem>

#include "ini_config.h"

using namespace std;
namespace fs = std::filesystem;

namespace jcq::components {

    static const auto TAG = u8"配置";

    static void override_config(json &config, const boost::property_tree::ptree &pt,
                                const initializer_list<string> &sections) {
        for (const auto &sec : sections) {
            try {
                const auto pt_child = pt.get_child(sec);
                for (const auto &item : pt_child) {
                    config[item.first] = item.second.get_value<string>();
                }
            } catch (...) {
                // section may not exist, skip it
            }
        }
    }

    static bool load_config(json &config, const string &filename, const initializer_list<string> &sections = {"JCQ"}) {
        const auto exts = {".cfg", ".ini"};

        boost::property_tree::ptree pt;
        auto loaded = false;

        for (const auto &ext : exts) {
            const auto filepath = filename + ext;
            if (fs::is_regular_file(ansi(filepath))) {
                try {
                    read_ini(ansi(filepath), pt);
                    override_config(config, pt, sections);
                    loaded = true;
                    break;
                } catch (boost::property_tree::ini_parser_error &) {
                    // syntax error
                    logging::fatal(TAG, u8"配置文件语法错误，路径：" + filepath);
                } catch (...) {
                    // failed to read
                }
            }
        }

        return loaded;
    }

    void IniConfig::hook_start(Context &ctx) {
        auto loaded = ctx.config->get_bool("_LOADED", false);

        const auto login_id_str = to_string(cq::get_login_user_id());

        if (!loaded) {
            // try <appid>\conf\JCQ.(cfg|ini)
            const auto filename = utils::dir::conf() + "JCQ";
            loaded = load_config(ctx.config->raw, filename);
        }

        ctx.config->raw["_LOADED"] = loaded;
        ctx.next();
    }

} // namespace jcq::components