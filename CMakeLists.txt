cmake_minimum_required(VERSION 3.10)

# 添加 vcpkg 工具链配置
# if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE
            "${CMAKE_SOURCE_DIR}/thirdparty_install/vcpkg/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "Vcpkg toolchain file")
    set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "Vcpkg triplet")
    include("${CMAKE_SOURCE_DIR}/thirdparty_install/vcpkg/scripts/buildsystems/vcpkg.cmake")
# endif()

project("FlowchartEditor" LANGUAGES CXX)

# 标准配置
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/Zc:__cplusplus)
endif ()

# 输出目录配置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 确保优先使用vcpkg中的包
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# 添加子目录
add_subdirectory(src)

# 获取所有插件目标并添加为Main的依赖
get_property(ALL_PLUGINS_TARGETS GLOBAL PROPERTY ALL_PLUGINS_TARGETS)

if(ALL_PLUGINS_TARGETS)
    add_dependencies(Main ${ALL_PLUGINS_TARGETS})
endif()

# 设置启动项目（Visual Studio）
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Main)

# 设置Main为默认构建目标
set_property(TARGET Main PROPERTY OUTPUT_NAME "FlowchartEditor")

# 添加控制台子系统（仅限 Windows）
if (WIN32)
    set_target_properties(Main PROPERTIES
            LINK_FLAGS "/SUBSYSTEM:CONSOLE"  # 强制显示控制台
    )
endif ()

# 如果是Debug模式，可以设置调试相关配置
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_property(TARGET Main PROPERTY 
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endif()