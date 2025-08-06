cmake_minimum_required(VERSION 3.10)

project("FlowchartEditor" LANGUAGES CXX)

set(CMAKE_TOOLCHAIN_FILE
        "${CMAKE_SOURCE_DIR}/thirdparty_install/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "Vcpkg triplet")
include("${CMAKE_SOURCE_DIR}/thirdparty_install/vcpkg/scripts/buildsystems/vcpkg.cmake")

# 标准配置
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 输出目录配置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 添加子目录
add_subdirectory(src)
# 获取所有插件目标并添加为Main的依赖
get_property(ALL_PLUGINS_TARGETS GLOBAL PROPERTY ALL_PLUGINS_TARGETS)

if(ALL_PLUGINS_TARGETS)
    add_dependencies(Main ${ALL_PLUGINS_TARGETS})
endif()

# 设置启动项目（假设主程序目标名为Main）
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Main)