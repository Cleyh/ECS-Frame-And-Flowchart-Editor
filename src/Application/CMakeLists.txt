cmake_minimum_required(VERSION 3.10)  #cmake版本最低3.10

project(Application LANGUAGES CXX)

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

qt5_wrap_ui(UI_HEADERS ${UI_FILES})

add_library(${PROJECT_NAME} STATIC
        ${HEADER_FILES}
        ${CPP_FILES}
        ${UI_FILES}
        ${RCC_FILES}
)

# 接口定义
#target_include_directories(${PROJECT_NAME} PUBLIC
#        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
#        $<INSTALL_INTERFACE:include>
#)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt5::Widgets
        Qt5::Core
        Qt5::Gui
        Frame
)