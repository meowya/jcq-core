//
// Created by Sobte on 2019-11-26.
//
#pragma once

#include "javacq/core/common.h"

namespace jcq::utils::java {

    std::vector<std::string> get_vm_options(const std::string &vm_name, bool is_custom = false);

} // namespace jcq::utils::java