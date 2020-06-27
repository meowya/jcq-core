//
// Created by Sobte on 2020-01-16.
//
#pragma once

#include "javacq/core/common.h"
#include "javacq/core/java.h"

namespace jcq::utils::jni {

    using jcq::java::jni::exception::JNIApiError;

    struct JNIEnvEx {
        JNIEnv *raw;

        JNIEnvEx() : raw(jcq::java::jni::get_env()) {}

        // Support "." Split
        jclass get_class(const std::string &name) const {
            auto out_name = name;
            string_replace(out_name, ".", "/");
            const auto result = raw->FindClass(out_name.c_str());
            if (!result) {
                throw JNIApiError(JNIApiError::INVALID_DATA, u8"未查找到此类：" + name);
            }
            return result;
        }

        jmethodID get_method_id(const jclass &clazz, const std::string &name, const std::string &sig) const {
            const auto result = raw->GetMethodID(clazz, name.c_str(), sig.c_str());
            if (!result) {
                throw JNIApiError(JNIApiError::INVALID_DATA, u8"未查找到此方法：" + name + sig);
            }
            return result;
        }

    };

} // namespace jcq::utils::jni