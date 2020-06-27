//
// Created by Sobte on 2019-11-28.
//
#pragma once

#include "javacq/core/common.h"

namespace jcq::utils::dir {

    std::string app(const std::string &sub_dir_name = "");

    std::string conf(const std::string &sub_dir_name = "");

    std::string bin(const std::string &sub_dir_name = "");

} // namespace jcq::utils::dir