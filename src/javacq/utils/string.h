//
// Created by Sobte on 2019-12-04.
//
#pragma once

#include "javacq/core/common.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>

namespace jcq::utils {
    inline std::optional<bool> to_bool(const std::string &str) {
        const auto s = boost::algorithm::to_lower_copy(str);
        if (s == "yes" || s == "true" || s == "1") {
            return true;
        }
        if (s == "no" || s == "false" || s == "0") {
            return false;
        }
        return std::nullopt;
    }

    inline bool to_bool(const std::string &str, const bool default_val) {
        auto result = to_bool(str);
        return result ? result.value() : default_val;
    }

    inline bool string_start_with(const std::string &str, const std::string &prefix, const int offset = 0) {
        return str.find(prefix, offset) == 0;
    }

} // namespace jcq::utils