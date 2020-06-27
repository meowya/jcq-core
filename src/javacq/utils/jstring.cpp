//
// Created by Sobte on 2019-11-04.
//
#include "javacq/core/java.h"

#include "jstring.h"

using namespace std;
using namespace jcq::java::jni;

namespace jcq::utils::jni {

    jstring string_to_java(const string &str) {
        return get_env()->NewStringUTF(str.data());// utf8
    }

    string string_from_java(const jstring &str) {
        string result;

        auto size = get_env()->GetStringUTFLength(str);
        if (size == 0) {
            return result;
        }

        const char *in = get_env()->GetStringUTFChars(str, JNI_FALSE);
        result = in;
        get_env()->ReleaseStringUTFChars(str, in);

        return result;// utf8
    }

    jstring wstring_to_java(const wstring &str) {
        return get_env()->NewString(reinterpret_cast<const jchar *>(str.data()), str.size());// utf16
    }

    wstring wstring_from_java(const jstring &str) {
        wstring result;

        auto size = get_env()->GetStringLength(str);
        if (size == 0) {
            return result;
        }

        const jchar *in = get_env()->GetStringChars(str, JNI_FALSE);
        result = reinterpret_cast<const wchar_t *>(in);
        get_env()->ReleaseStringChars(str, in);

        return result;// utf16
    }

} // namespace jcq::utils::jni