#include "BrokenLineItem.h"

#include "qgraphicssceneevent.h"
#include "qvector2d.h"

BrokenLineItem::BrokenLineItem(const QRectF &rect, QGraphicsItem *parent)
    : CanvasElement(rect, parent) {
    pluginName = "PathBrokenLine";
    CanvasElement::setType(TypeDefine::ToolType::PATH);
    CanvasElement::getProperties()->setProperty("closed", false);
}

QPainterPath BrokenLineItem::constructShape() const {
    const auto closed = getProperties()->getProperty("closed").toBool();
    // 绘制每个点
    QPainterPath path;
    if (!points.isEmpty()) {
        path.moveTo(points.first().point());
        for (int i = 1; i < points.size(); ++i) {
            path.lineTo(points[i].point());
        }
    }
    // 闭合多边形
    if (closed) path.closeSubpath();
    return path;
}
