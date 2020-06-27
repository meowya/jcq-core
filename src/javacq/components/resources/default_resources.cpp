//
// Created by Sobte on 2019-12-06.
//

#include <filesystem>

#include "javacq/utils/http.h"
#include "javacq/utils/helper.h"

#include "default_resources.h"

using namespace std;

namespace fs = std::filesystem;

namespace jcq::components {

    static const auto TAG = u8"资源更新";

    static bool download_resource(const string &name, const string &file_name) {
        string resource_url = utils::helper::get_resources_url(name);
        string tmp_path = file_name + ".tmp";
        if (!utils::http::download(resource_url, tmp_path)) {
            // download failed
            return false;
        }
        try {
            copy_file(ansi(tmp_path), ansi(file_name), fs::copy_options::overwrite_existing);
            fs::remove(ansi(tmp_path));
            return true;
        } catch (fs::filesystem_error &) {
            return false;
        }
    }

    void DefaultResources::hook_start(Context &ctx) {
        logging::debug(TAG, u8"正在检查相关依赖资源");
        for (const auto &name : utils::helper::get_resources_list()) {
            string path = utils::dir::bin() + name;
            if (!fs::exists(path)) {
                logging::debug(TAG, u8"正在下载资源 (" + name + u8") ...");
                if (!download_resource(name, path)) {
                    logging::error(TAG, u8"依赖资源 (" + name + u8") 下载失败，请检查网络连接是否通畅，或尝试更换数据源。");
                    continue;
                }
                logging::debug(TAG, u8"资源 (" + name + u8") 下载安装完成");
            }
        }
        logging::debug(TAG, u8"相关依赖资源检测完成");
        ctx.next();
    }

} // namespace jcq::components