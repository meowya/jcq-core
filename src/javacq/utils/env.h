//
// Created by Sobte on 2020-01-14.
//
#pragma once

#include "javacq/core/common.h"

namespace jcq::utils::env {

    bool is_in_wine();

    std::string get_registry_val(const std::string &path, const std::string &key);

    std::string get_env_variable(const std::string &key, const std::string &default_val = "");

}// namespace jcq::utils::env