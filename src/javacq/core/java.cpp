//
// Created by Sobte on 2019-11-04.
//
#include <filesystem>

#include <javacq/utils/memory.h>

#include "java.h"

using namespace std;

namespace fs = std::filesystem;

namespace jcq::java::jni {

    void __init(const string &jre_path) {
        auto jre_bin_path = fs::path(jre_path) / "bin" / "";
        auto jvm_path = jre_bin_path / "server" / "jvm.dll";
        if (!fs::exists((jvm_path).string())) {
            jvm_path = jre_bin_path / "client" / "jvm.dll";
        }
        if (!fs::exists(jvm_path.string())) {
            throw jcq::exception::RuntimeError(u8"加载JVM相关文件失败，相关文件不存在，请配置正确的jre路径");
        }
        AddDllDirectory(s2ws(jre_bin_path.string()).c_str()); // Add Java Home Bin
        AddDllDirectory(s2ws((jre_bin_path / "plugin2" / "").string()).c_str()); // Add Java Home plugin2
        AddDllDirectory(s2ws((jre_bin_path / "dtplugin" / "").string()).c_str()); // Add Java Home dtplugin
        const auto dll = LoadLibraryW(s2ws(jvm_path.string()).c_str()); // JVM Core
        int error_code = GetLastError();
        if (!dll) {
            throw exception::JNIApiError(error_code,
                                         u8"加载JVM相关文件失败 (GetLastError," + to_string(error_code) + u8")");
        }
        raw::JNI_CreateJavaVM = reinterpret_cast<raw::__JNI_CreateJavaVM_T>(GetProcAddress(dll, u8"JNI_CreateJavaVM"));
        if (!raw::JNI_CreateJavaVM) {
            error_code = GetLastError();
            FreeLibrary(dll);
            throw exception::JNIApiError(error_code,
                                         u8"加载JVM相关文件失败 (GetLastError," + to_string(error_code) + u8")");
        }
    }

    namespace raw {
        JavaVM *java_vm;

        __JNI_CreateJavaVM_T JNI_CreateJavaVM;
    } // namespace raw

    void create_java_vm(const vector<string> &args, const jint &version) {
        JavaVMInitArgs java_vm_args;
        JNIEnv *env;
        const auto options = jcq::utils::make_shared_array<JavaVMOption>(args.size()).get();
        unsigned i = 0;
        for (const auto &arg : args) {
            options[i].optionString = const_cast<char *>(arg.c_str());
            ++i;
        }
        java_vm_args.version = version;
        java_vm_args.nOptions = args.size();
        java_vm_args.options = options;
        java_vm_args.ignoreUnrecognized = false;
        jint result = raw::JNI_CreateJavaVM(&raw::java_vm, reinterpret_cast<void **>(&env), &java_vm_args);
        if (result != JNI_OK) {
            throw exception::JNIApiError(result, u8"Java虚拟机构建失败 (" + to_string(result) + u8")");
        }
    }

} // namespace jcq::java::jni