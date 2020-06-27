//
// Created by Sobte on 2019-12-04.
//
#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include <cqcppsdk/cqcppsdk.h>

using cq::utils::ansi;
using cq::utils::s2ws;
using cq::utils::ws2s;
using cq::utils::string_replace;
using cq::utils::string_ltrim;
using cq::utils::string_rtrim;
using cq::utils::string_trim;

namespace logging = cq::logging;

#include <Windows.h>

#include "javacq/utils/jstring.h"

using jcq::utils::jni::string_to_java;
using jcq::utils::jni::string_from_java;
using jcq::utils::jni::wstring_to_java;
using jcq::utils::jni::wstring_from_java;

#include "javacq/core/def.h"
#include "javacq/core/json_convertor.h"
#include "javacq/utils/dir.h"
