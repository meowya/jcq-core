//
// Created by Sobte on 2019-12-04.
//
#include "javacq/core/common.h"

#include "javacq/utils/string.h"

namespace jcq::utils {
    struct JsonEx {
        json raw;

        JsonEx() : raw(json::object()) {}

        explicit JsonEx(const json &j) : raw(j) {}

        /**
         * Put a key-value pair into the json object.
         * If "raw" is not an object, a json::type_error may be throwed.
         */
        void put(const std::string &key, const json &value) { raw[key] = value; }

        /**
         * Return std::nullopt if the key does not exist.
         */
        std::optional<json> get(const std::string &key) const {
            if (const auto it = raw.find(key); it != raw.end()) {
                return *it;
            }
            return std::nullopt;
        }

        template<typename Type>
        std::optional<Type> get(const std::string &key) const {
            auto v = get(key);
            if (v) {
                try {
                    return v->get<Type>();
                } catch (json::exception &) {
                    // type doesn't match
                }
            }
            return std::nullopt;
        }

        std::string get_string(const std::string &key, const std::string &default_val = "") const {
            return get<std::string>(key).value_or(default_val);
        }

        jstring get_jstring(const std::string &key, const std::string &default_val = "") const {
            return string_to_java(get_string(key, default_val));
        }

        int64_t get_integer(const std::string &key, const int64_t default_val = 0) const {
            auto result = default_val;
            if (auto v_opt = get(key); v_opt && v_opt->is_string()) {
                try {
                    result = stoll(v_opt->get<std::string>());
                } catch (std::invalid_argument &) {
                    // invalid integer string
                }
            } else if (v_opt->is_number_integer()) {
                result = v_opt->get<int64_t>();
            }
            return result;
        }

        bool get_bool(const std::string &key, const bool default_val = false) const {
            auto result = default_val;
            if (auto v_opt = get(key); v_opt && v_opt->is_string()) {
                result = to_bool(v_opt->get<std::string>(), default_val);
            } else if (v_opt->is_boolean()) {
                result = v_opt->get<bool>();
            }
            return result;
        }

    };

    inline void from_json(const json &j, JsonEx &jsonex) { jsonex.raw = j; }

    inline void to_json(json &j, const JsonEx &jsonex) { j = jsonex.raw; }

} // namespace jcq::utils