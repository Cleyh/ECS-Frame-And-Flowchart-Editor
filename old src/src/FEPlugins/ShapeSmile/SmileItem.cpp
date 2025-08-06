//
// Created by unbd_ on 2025/5/3.
//

#include "smileitem.h"
#include <qdebug.h>

SmileItem::SmileItem(const QRectF &rect, QGraphicsItem *parent)
    : CanvasElement(rect, parent) {
    pluginName = "ShapeSmile";
}

void SmileItem::paint(QPainter *painter,
                      const QStyleOptionGraphicsItem *option,
                      QWidget *widget) {
    CanvasElement::paint(painter, option, widget);
}

QPainterPath SmileItem::constructShape() const {
    QPainterPath path;
    QRectF rect(points[0], points[1]);
    auto calcRect = rect;
    this->rect = rect;
    path.addEllipse(calcRect);

    // ================= 计算特征比例 =================
    const qreal eyeWidth = calcRect.width() * 0.15;
    const qreal eyeHeight = calcRect.height() * 0.2;
    const qreal eyeYPos = calcRect.top() + calcRect.height() * 0.3;
    const qreal eyeXOffset = calcRect.width() * 0.25;

    // ================= 绘制左眼 =================
    QRectF leftEye(
        calcRect.left() + eyeXOffset - eyeWidth / 2, // x
        eyeYPos - eyeHeight / 2, // y
        eyeWidth, // width
        eyeHeight // height
    );

    // ================= 绘制右眼 =================
    QRectF rightEye(
        calcRect.right() - eyeXOffset - eyeWidth / 2, // x
        eyeYPos - eyeHeight / 2, // y
        eyeWidth, // width
        eyeHeight // height
    );
    path.addEllipse(leftEye);
    path.addEllipse(rightEye);

    // ================= 绘制嘴巴 =================
    const qreal mouthWidth = calcRect.width() * 0.6;
    const qreal mouthHeight = calcRect.height() * 0.4;
    QRectF mouthRect(
        calcRect.center().x() - mouthWidth / 2, // x
        calcRect.top() + calcRect.height() * 0.5, // y
        mouthWidth, // width
        mouthHeight // height
    );
    path.addEllipse(mouthRect);

    return path;
}

SmileItem *SmileItem::reserialize(const BasicElement &element) {
    return new SmileItem();
}
