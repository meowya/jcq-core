//
// Created by Sobte on 2019-11-04.
//
#pragma once

#include "javacq/core/common.h"
#include "javacq/core/exception.h"

#include <jni.h>

namespace jcq::java::jni::exception {
    struct JNIApiError : jcq::exception::RuntimeError {
        int code;

        explicit JNIApiError(const int code) : RuntimeError(
                "failed to call jni api (" + std::to_string(code) + ")") { this->code = code; }

        explicit JNIApiError(const int code, const std::string &what_arg) :
                RuntimeError(what_arg) { this->code = code; }

        static const auto INVALID_JVM = 1000;
        static const auto INVALID_DATA = 1010;
    };
} // jcq::java::jni::exception

namespace jcq::java::jni {

    void __init(const std::string &jre_path);

    namespace raw {
        extern JavaVM *java_vm;

        // 定义一个函数指针，下面用来指向JVM中的JNI_CreateJavaVM函数
        typedef jint(JNICALL *__JNI_CreateJavaVM_T)(JavaVM **pvm, void **penv, void *args);

        // JNI Create JavaVM
        extern __JNI_CreateJavaVM_T JNI_CreateJavaVM;
    } // namespace raw

    inline void __throw_if_needed(const int32_t ret) noexcept(false) {
        if (ret < 0) {
            throw exception::JNIApiError(ret);
        }
    }

    inline void __throw_if_needed(const void *const ret_ptr) noexcept(false) {
        if (!ret_ptr) {
            throw exception::JNIApiError(exception::JNIApiError::INVALID_DATA);
        }
    }

    // 创建JVM
    void create_java_vm(const std::vector<std::string> &args = {}, const jint &version = JNI_VERSION_1_8);

    // 释放JVM
    inline void destroy_java_vm() {
        __throw_if_needed(raw::java_vm->DestroyJavaVM());
    }

    // 附加当前线程到JVM守护线程
    inline JNIEnv *attach_current_thread_as_daemon() {
        JNIEnv *env;
        raw::java_vm->AttachCurrentThreadAsDaemon(reinterpret_cast<void **>(&env),
                                                  nullptr);
        __throw_if_needed(env);
        return env;
    }

    // 附加当前线程到JVM守护线程
    inline JNIEnv *attach_current_thread_as_daemon(const JavaVMAttachArgs &args) {
        JNIEnv *env;
        raw::java_vm->AttachCurrentThreadAsDaemon(reinterpret_cast<void **>(&env),
                                                  reinterpret_cast<void *>(const_cast<JavaVMAttachArgs *>(&args)));
        __throw_if_needed(env);
        return env;
    }

    // 附加当前线程到JVM线程
    inline JNIEnv *attach_current_thread() {
        JNIEnv *env;
        raw::java_vm->AttachCurrentThread(reinterpret_cast<void **>(&env),
                                          nullptr);
        __throw_if_needed(env);
        return env;
    }

    // 附加当前线程到JVM线程
    inline JNIEnv *attach_current_thread(const JavaVMAttachArgs &args) {
        JNIEnv *env;
        raw::java_vm->AttachCurrentThread(reinterpret_cast<void **>(&env),
                                          reinterpret_cast<void *>(const_cast<JavaVMAttachArgs *>(&args)));
        __throw_if_needed(env);
        return env;
    }

    // 移除当前附加线程
    inline void detach_current_thread() {
        __throw_if_needed(raw::java_vm->DetachCurrentThread());
    }

    // 获取当前线程的操作对象
    inline JNIEnv *get_env(const jint &version = JNI_VERSION_1_8) {
        JNIEnv *env;
        raw::java_vm->GetEnv(reinterpret_cast<void **>(&env), version);
        __throw_if_needed(env);
        return env;
    }

} // namespace jcq::java::jni