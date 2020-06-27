#pragma once

#include <memory>

namespace jcq::utils {
    template <typename T>
    inline std::shared_ptr<T> make_shared_array(const size_t size) {
        return std::shared_ptr<T>(new T[size], [](T *p) { delete[] p; });
    }
} // namespace jcq::utils
