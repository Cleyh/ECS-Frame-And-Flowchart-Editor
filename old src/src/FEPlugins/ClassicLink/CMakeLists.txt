cmake_minimum_required(VERSION 3.10)  #cmake版本最低3.10

# 获取当前子CMake文件所在目录的完整路径
get_filename_component(CURRENT_DIR_PATH "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
# 从完整路径中提取目录名称
get_filename_component(CURRENT_DIR_NAME "${CURRENT_DIR_PATH}" NAME)
project(${CURRENT_DIR_NAME} LANGUAGES CXX)

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
file(GLOB UI_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.ui")
file(GLOB RCC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*qrc")
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_BINARY_DIR}/*.h")
file(GLOB CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

qt5_wrap_ui(UI_HEADERS ${UI_FILES})
qt5_add_resources(RCC_SOURCES ${RCC_FILES})

source_group("Form Files" FILES ${UI_FILES})
source_group("Resource Files" FILES ${RCC_FILES})

add_library(${PROJECT_NAME} SHARED
        ${HEADER_FILES}
        ${CPP_FILES}
        ${UI_FILES}
        ${RCC_SOURCES}
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        DEFINE_SYMBOL "FEPLUGINS_EXPORTS"  # 自动定义该宏
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/plugins
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/plugins
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        FEPLUGINS_EXPORTS
)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt5::Widgets
        Qt5::Core
        Qt5::Gui
        IPlugin
        GraphicsComponents
)