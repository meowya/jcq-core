//
// Created by Sobte on 2019-11-26.
//

#include <filesystem>
#include <fstream>

#include "options.h"

#include "helper.h"

using namespace std;

namespace fs = std::filesystem;

namespace jcq::utils::java {

    static const vector<string> DEFAULT_OPTIONS = {
            "-Xrs",
            "-Xss1m",
            "-Xms32m",
            "-Xmx512m",
            "-XX:NewSize=2m",
            "-XX:MaxNewSize=128m",
            "-XX:PermSize=64m",
            "-XX:MaxPermSize=256m",
            "-Dfile.encoding=UTF-8"
    };

    static const auto TAG = u8"配置";

    static void create_options(const string &path) {
        if (ofstream file(path); file.is_open()) {
            for (const auto &option : DEFAULT_OPTIONS) {
                file << option << endl;
            }
            file.close();
        } else {
            logging::error(TAG, u8"配置写入失败，请检查文件系统权限");
        }
    }

    static string get_class_path() {
        string resources_path = "-Djava.class.path=";
        for (const auto &name : helper::get_resources_list()) {
            resources_path += dir::bin() + name + ";";
        }
        return resources_path;
    }

    static bool accept_option(const string &option) {
        for (const auto &opt : DEFAULT_OPTIONS) {
            string::size_type position = opt.find('=');
            if (position != std::string::npos && option.find(opt.substr(0, position)) == 0) {
                return true;
            }
        }

        return option == "-Xrs" || option.find("-Xss") == 0
               || option.find("-Xms") == 0 || option.find("-Xmx") == 0;

    }

    vector<string> get_vm_options(const string &vm_name, const bool is_custom) {
        vector<string> options;
        string options_path = dir::conf() + vm_name + ".vmoptions";

        if (!fs::exists(options_path)) {// options not exists
            create_options(options_path);
            options = DEFAULT_OPTIONS;
            options.push_back(get_class_path());
            return options;
        }

        string class_path;
        if (ifstream f(options_path, ios::in | ios::binary); f.is_open()) {
            string line;
            while (getline(f, line)) {
                string_trim(line);
                if (line.empty()) {
                    continue;
                }
                if (line.find("-Djava.class.path=") == 0) {
                    class_path = line;
                    continue;
                }

                if (is_custom || accept_option(line)) {
                    options.push_back(line);
                }
            }
            f.close();// close file
        }

        if (is_custom) {
            string_replace(class_path, "-Djava.class.path=", get_class_path());
            options.push_back(class_path);
            return options;
        }

        options.push_back(get_class_path());

        return options;
    }

} // jcq::utils::java