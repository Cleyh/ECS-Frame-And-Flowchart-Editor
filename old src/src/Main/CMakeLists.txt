cmake_minimum_required(VERSION 3.10)  #cmake版本最低3.10

project(Main LANGUAGES CXX)

# 标准配置
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/Zc:__cplusplus)
endif ()

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

# 文件收集
file(GLOB_RECURSE UI_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.ui")
file(GLOB_RECURSE RCC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*qrc")
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_BINARY_DIR}/*.h")
file(GLOB_RECURSE CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE QSS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.qss")

qt5_wrap_ui(UI_HEADERS ${UI_FILES})
qt5_add_resources(RCC_SOURCES ${RCC_FILES})

source_group("Form Files" FILES ${UI_FILES})
source_group("Resource Files" FILES ${RCC_FILES})
# source_group("Head Files" FILES ${HEADER_FILES})
# source_group("CPP Files" FILES ${CPP_FILES})
source_group("QSS Files" FILES ${QSS_FILES})

# 定义宏处理文件分组
function(organize_files)
    set(options)
    set(oneValueArgs ROOT PREFIX)
    set(multiValueArgs FILES)
    cmake_parse_arguments(ORG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    foreach (file IN LISTS ORG_FILES)
        # 获取相对路径
        file(RELATIVE_PATH rel_path ${ORG_ROOT} ${file})

        # 获取目录结构
        get_filename_component(dir_path ${rel_path} DIRECTORY)
        if (dir_path STREQUAL "")
            set(group_path "${ORG_PREFIX}")
        else ()
            # 转换路径分隔符为IDE兼容格式
            string(REPLACE "/" "\\" group_path "${ORG_PREFIX}\\${dir_path}")
        endif ()

        # 创建分组
        source_group("${group_path}" FILES ${file})
    endforeach ()
endfunction()

# 应用分组逻辑
organize_files(
        ROOT ${CMAKE_CURRENT_SOURCE_DIR}
        PREFIX "Head Files"
        FILES ${HEADER_FILES}
)

organize_files(
        ROOT ${CMAKE_CURRENT_SOURCE_DIR}
        PREFIX "CPP Files"
        FILES ${CPP_FILES}
)

add_executable(${PROJECT_NAME} WIN32
        ${HEADER_FILES}
        ${CPP_FILES}
        ${UI_FILES}
        ${RCC_SOURCES}
        ${QSS_FILES}
)

# 接口定义
#target_include_directories(${PROJECT_NAME} PUBLIC
#        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
#        $<INSTALL_INTERFACE:include>
#)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt5::Widgets
        Qt5::Core
        Qt5::Gui
        GraphicsComponents
        IPlugin
        Utils
        DataModels
)

# 在 CMakeLists.txt 中，添加控制台子系统（仅限 Windows）
if (WIN32)
    set_target_properties(Main PROPERTIES
            LINK_FLAGS "/SUBSYSTEM:CONSOLE"  # 强制显示控制台
    )
endif ()