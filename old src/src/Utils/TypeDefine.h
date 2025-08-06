#ifndef TYPEDEFINE_H
#define TYPEDEFINE_H

#include <QColor>
namespace TypeDefine {
    enum class BasicPropertyType {
        STRING,
        INT,
        FLOAT,
        BOOL,
        COLOR,
        ENUM
    };

    inline int ELEMENT_MIN_SIZE = 50; // 最小图元尺寸
    /**
     * - SELECT工具
     * - SHAPE工具 - 使用时创建固定大小矩形，以矩形生成图形
     * - PATH工具 - 使用时创建大小为0的矩形，PATH Item会自动以矩形tl和br创建两个点
     * - LINK工具 - 使用时创建大小为0的矩形，同时生成两个链接点
     */
    enum class ToolType {
        SELECT,
        SHAPE,
        PATH,
        LINK
    };


    enum class PointType {
        DEFAULT,
        CONTROL,
        LINK
    };

    inline constexpr auto HOVER_COLOR = QColor(74, 134, 251, 128); // 半透明蓝色
    inline constexpr auto RED_COLOR = QColor(252, 95, 95, 128); // 半透明红色
}

#endif //TYPEDEFINE_H
