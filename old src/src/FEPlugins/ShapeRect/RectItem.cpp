//
// Created by unbd_ on 2025/5/5.
//

#include "RectItem.h"

#include "TypeDefine.h"

RectItem::RectItem(const QRectF &rect, QGraphicsItem *parent)
    : CanvasElement(rect, parent) {
    pluginName = "ImageContainer";
    CanvasElement::getProperties()->setProperty("image_src", "");
    CanvasElement::getProperties()->setProperty("border_radius", 10);
}

void RectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    CanvasElement::paint(painter, option, widget);
    QString src = getProperties()->getProperty("image_src").toString();
    auto closed = getProperties()->getProperty("closed").toBool();
    if (src.isEmpty()) {
        return;
    }
    QPixmap pixmap(src);
    if (pixmap.isNull()) {
        // 图片加载失败时绘制红色警告框
        if (closed) painter->setBrush(TypeDefine::RED_COLOR);
        painter->drawRect(rect);
        painter->setPen(Qt::red);
        painter->drawText(rect, Qt::AlignCenter, src + " load failed!");
        return;
    }
    // 缩放图片以适应目标矩形，保持宽高比
    QPixmap scaledPixmap = pixmap.scaled(
        rect.size().toSize(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
    );
    // 计算居中位置
    QPointF drawPos(
        rect.x() + (rect.width() - scaledPixmap.width()) / 2,
        rect.y() + (rect.height() - scaledPixmap.height()) / 2
    );
    // 绘制图片
    painter->drawPixmap(drawPos, scaledPixmap);
}

QPainterPath RectItem::constructShape() const {
    QPainterPath path;
    qreal borderRadius = getProperties()->getProperty("border_radius").toReal();
    path.setFillRule(Qt::WindingFill);
    QRectF rect(points[0], points[1]);
    path.addRoundedRect(rect, borderRadius, borderRadius);
    this->rect = rect;
    return path;
}
