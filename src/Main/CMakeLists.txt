project(Main LANGUAGES CXX)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

# 收集源文件 #
# cpp
file(GLOB_RECURSE CPP_FILES
    "${CMAKE_CURRENT_LIST_DIR}/*.cpp"
)
# h
file(GLOB_RECURSE HEADER_FILES
    "${CMAKE_CURRENT_LIST_DIR}/*.h"
    "${CMAKE_CURRENT_BINARY_DIR}/*.h"
)
# ui
file(GLOB_RECURSE UI_FILES
    "${CMAKE_CURRENT_LIST_DIR}/*.ui"
)
# qrc
file(GLOB_RECURSE RCC_FILES
    "${CMAKE_CURRENT_LIST_DIR}/*.qrc"
)
# qss
file(GLOB_RECURSE QSS_FILES 
    "${CMAKE_CURRENT_LIST_DIR}/*.qss"
) 
# 收集文件 #

qt5_wrap_ui(UI_HEADERS ${UI_FILES})
qt5_add_resources(RCC_SOURCES ${RCC_FILES})

add_executable(${PROJECT_NAME} WIN32
    ${CPP_FILES} ${HEADER_FILES} ${UI_FILES} ${RCC_FILES} ${QSS_FILES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt5::Widgets
        Qt5::Core
        Qt5::Gui
        Application
        Frame
)