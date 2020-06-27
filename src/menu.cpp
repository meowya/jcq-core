//
// Created by Sobte on 2020/2/11.
//
#include "javacq/core/core.h"

using namespace cq;

CQ_MENU(menu_app_dir) {
    ShellExecute(nullptr, "open", nullptr, nullptr, cq::dir::app("app").c_str(), SW_SHOWDEFAULT);
}

CQ_MENU(menu_conf_dir) {
    ShellExecute(nullptr, "open", nullptr, nullptr, cq::dir::app("conf").c_str(), SW_SHOWDEFAULT);
}

CQ_MENU(menu_console) {
    // TODO open console
}