//
// Created by Sobte on 2019-11-28.
//

#include <filesystem>

#include "dir.h"

using namespace std;

namespace fs = std::filesystem;

namespace jcq::utils::dir {

    static void create_dir_if_not_exists(const string &dir) {
        const auto ansi_dir = ansi(dir);
        if (!fs::exists(ansi_dir)) {
            fs::create_directories(ansi_dir);
        }
    }

    static const auto conf_dir_name = "conf";
    static const auto bin_dir_name = "bin";

    string conf(const string &sub_dir_name) {
        if (sub_dir_name.empty()) {
            return cq::dir::app(conf_dir_name);
        }
        const auto dir = cq::dir::app(conf_dir_name) + (sub_dir_name.empty() ? "" : sub_dir_name + "\\");
        create_dir_if_not_exists(dir);
        return dir;
    }

    string bin(const string &sub_dir_name) {
        if (sub_dir_name.empty()) {
            return cq::dir::app(bin_dir_name);
        }
        const auto dir = cq::dir::app(bin_dir_name) + (sub_dir_name.empty() ? "" : sub_dir_name + "\\");
        create_dir_if_not_exists(dir);
        return dir;
    }

    string app(const string &sub_dir_name) {
        if (sub_dir_name.empty()) {
            return cq::dir::app("app");
        }
        const auto dir = cq::dir::app("app") + (sub_dir_name.empty() ? "" : sub_dir_name + "\\");
        create_dir_if_not_exists(dir);
        return dir;
    }

} // namespace jcq::utils::dir