cmake_minimum_required(VERSION 3.9)
project(jcq-core) # 填写项目名称

set(CMAKE_CXX_STANDARD 17) # 设置 C++ 标准版本为 C++17, 必须

include(extern/cqcppsdk/cqcppsdk.cmake) # 包含 SDK 的 CMake 脚本, 必须

# policy
cmake_policy(SET CMP0074 NEW)

# json
include_directories(extern/nlohmann_json/include)

# jni
set(JNI_MIN_VERSION 1.8)
find_package(JNI ${JNI_MIN_VERSION} REQUIRED)
if (NOT JNI_FOUND)
    message(FATAL_ERROR "Fatal error: JNI (version ${JNI_MIN_VERSION}) required.")
else ()
    message(STATUS "Setting up JNI")
    message(STATUS " Includes - ${JAVA_INCLUDE_PATH}")
    message(STATUS " Includes - ${JAVA_INCLUDE_PATH2}")
    message(STATUS " Library  - ${JAVA_JVM_LIBRARY}")
    include_directories(${JAVA_INCLUDE_PATH})
    include_directories(${JAVA_INCLUDE_PATH2})
    LINK_LIBRARIES(${JAVA_JVM_LIBRARY})
endif ()

# boost
set(BOOST_MIN_VERSION "1.71.0")
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Fatal error: Boost (version ${BOOST_MIN_VERSION}) required.")
else ()
    message(STATUS "Setting up BOOST")
    message(STATUS " Includes - ${Boost_INCLUDE_DIRS}")
    message(STATUS " Library  - ${Boost_LIBRARY_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})
    LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
endif ()

# openssl
set(OpenSSL_MIN_VERSION 1.1.1d)
find_package(OpenSSL REQUIRED)
if (NOT OpenSSL_FOUND)
    message(FATAL_ERROR "Fatal error: OpenSSL (version ${OpenSSL_MIN_VERSION}) required.")
else ()
    message(STATUS "Setting up OpenSSL")
    message(STATUS " Includes - ${OPENSSL_INCLUDE_DIR}")
    message(STATUS " Library - ${LIB_EAY_RELEASE}")
    message(STATUS " Library  - ${SSL_EAY_RELEASE}")
    LINK_LIBRARIES(${LIB_EAY_RELEASE} ${SSL_EAY_RELEASE})
endif ()

# curl
set(CURL_MIN_VERSION "7.67.0")
find_package(CURL ${CURL_MIN_VERSION} REQUIRED)
if (NOT CURL_FOUND)
    message(FATAL_ERROR "Fatal error: CURL (version ${CURL_MIN_VERSION}) required.")
else ()
    message(STATUS "Setting up CURL")
    message(STATUS " Includes - ${CURL_INCLUDE_DIRS}")
    message(STATUS " Library  - ${CURL_LIBRARIES}")
    include_directories(${CURL_INCLUDE_DIRS})
    LINK_LIBRARIES("$ENV{ZLIB_ROOT}/lib/zlib.lib")
    LINK_LIBRARIES(${CURL_LIBRARIES} ws2_32.lib winmm.lib wldap32.lib Crypt32.lib)
endif ()

# define
add_definitions(-D NOMINMAX)
add_definitions(-D CURL_STATICLIB)

include_directories(src) # 设置 src 目录为包含目录, 可根据实际情况修改

# 从 app_id.txt 文件读取 app id, 也可以直接设置
file(READ "app_id.txt" APP_ID)
string(STRIP "${APP_ID}" APP_ID)
cq_set_app_id(${APP_ID}) # 设置 app id, 必须

file(GLOB_RECURSE SOURCE_FILES src/*.cpp) # 递归搜索 src 目录中的源文件, 可根据实际情况修改

if (CQ_CAN_BUILD_STD_MODE)
    cq_add_std_app(${SOURCE_FILES}) # 添加 std 模式的动态链接库构建目标
    cq_install_std_app()
endif ()

# 添加 dev 模式的可执行文件构建目标
cq_add_dev_app(${SOURCE_FILES})
