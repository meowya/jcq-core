//
// Created by Sobte on 2019-11-04.
//
#pragma once

#include <string>
#include <jni.h>

namespace jcq::utils::jni {

    // utf8
    jstring string_to_java(const std::string &str);

    // utf8
    std::string string_from_java(const jstring &str);

    // utf16
    jstring wstring_to_java(const std::wstring &str);

    // utf16
    std::wstring wstring_from_java(const jstring &str);

} // namespace jcq::utils::jni
