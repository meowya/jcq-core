//
// Created by Sobte on 2019/12/15.
//
#include "javacq/core/common.h"

namespace jcq::utils::helper {

    std::string get_git_raw_url(const std::string &author, const std::string &project,
                                const std::string &path = "", const std::string &type = "coding");

    std::string get_source_url(const std::string &path = "");

    inline std::string get_version_url(const std::string &path = "", const std::string &version = JCQ_VERSION,
                                       const int build_number = JCQ_BUILD_NUMBER) {
        return get_source_url() + "versions/" + JCQ_VERSION_PRE_S + "/" +
               version + "_b" + std::to_string(JCQ_BUILD_NUMBER) + "/" +
               path;
    }

    inline std::string get_version_info_url(const std::string &version = JCQ_VERSION,
                                            const int build_number = JCQ_BUILD_NUMBER) {
        return get_version_url("info.json", version, build_number);
    }

    inline std::string get_resources_url(const std::string &path = "", const std::string &version = JCQ_VERSION,
                                         const int build_number = JCQ_BUILD_NUMBER) {
        return get_version_url("resources/", version, build_number) + path;
    }

    std::vector<std::string> get_resources_list(const std::string &version = JCQ_VERSION,
                                                const int build_number = JCQ_BUILD_NUMBER);

} // namespace jcq::utils::helper