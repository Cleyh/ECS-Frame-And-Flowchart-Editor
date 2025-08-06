# 获取src下所有子目录（不递归）
file(GLOB SUBDIRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*")

# 设置排除目录列表
set(EXCLUDE_DIRS ".git" "build" "CMakeFiles")

foreach(SUBDIR ${SUBDIRS})
    # 仅处理目录类型
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}")
        # 检查是否在排除列表
        if(NOT "${SUBDIR}" IN_LIST EXCLUDE_DIRS)
            # 检查CMakeLists.txt是否存在
            if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}/CMakeLists.txt")
                message(STATUS "Adding module: ${SUBDIR}")
                add_subdirectory(${SUBDIR})
            endif()
        endif()
    endif()
endforeach()

# 依赖管理
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)