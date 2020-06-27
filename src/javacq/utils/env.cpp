//
// Created by Sobte on 2020-01-14.
//
#include "env.h"

using namespace std;

namespace jcq::utils::env {

    static optional<bool> is_wine;

    bool is_in_wine() {
        if (is_wine.has_value()) {
            return is_wine.value();
        }
        auto tmp = false;
        const auto ntdll = GetModuleHandleW(L"ntdll.dll");
        if (ntdll) {
            if (GetProcAddress(ntdll, "wine_get_version")) {
                tmp = true;
            }
        } else {
            tmp = true;
        }
        is_wine = tmp;
        return tmp;
    }

    string get_registry_val(const string &path, const string &key) {
        string result;

        HKEY hkey_path;
        DWORD out_bytes_left = MAX_PATH;
        auto out = new wchar_t[out_bytes_left]{0};
        const auto out_begin = out;

        // open key path
        try {
            RegOpenKeyExW(HKEY_LOCAL_MACHINE, s2ws(path).c_str(), NULL, KEY_QUERY_VALUE, &hkey_path);
            if (RegGetValueW(hkey_path, nullptr, s2ws(key).c_str(), RRF_RT_ANY, nullptr, out, &out_bytes_left) ==
                ERROR_SUCCESS) {
                result = ws2s(out_begin);
            }
            RegCloseKey(hkey_path);
        } catch (...) {
        }
        // close key path

        delete[] out_begin;

        return result;
    }

    string get_env_variable(const string &key, const string &default_val) {
        string result = default_val;

        try {
            if (auto out = getenv(key.c_str())) {
                result = out;
            }
        } catch (...) {
        }

        return result;
    }


}// namespace jcq::utils::env