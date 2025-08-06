//
// Created by unbd_ on 2025/5/6.
//

#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H
#include "QList"
#include "QPainterPath"
#include "QLineF"
#include "QPointF"

namespace CommonFunctions {
    inline QPointF cubicBezierPoint(QPointF p0, QPointF p1, QPointF p2, QPointF p3, qreal t) {
        qreal u = 1 - t;
        qreal uu = u * u;
        qreal uuu = uu * u;
        qreal tt = t * t;
        qreal ttt = tt * t;

        return uuu * p0
               + 3 * uu * t * p1
               + 3 * u * tt * p2
               + ttt * p3;
    }

    // 直线段二分法细分
    inline void subdivideLine(const QPointF &start, const QPointF &end,
                              QList<QPointF> &points, qreal tolerance, int depth) {
        if (depth <= 0 || QLineF(start, end).length() < tolerance) {
            points.append(end);
            return;
        }

        QPointF mid = (start + end) / 2;
        subdivideLine(start, mid, points, tolerance, depth - 1); // 左半段
        subdivideLine(mid, end, points, tolerance, depth - 1); // 右半段
    }

    // 贝塞尔曲线二分法细分（基于弦高误差）
    inline void subdivideBezier(const QPointF &p0, const QPointF &p1, const QPointF &p2, const QPointF &p3,
                                QList<QPointF> &points, qreal tolerance, int depth) {
        if (depth <= 0) {
            points.append(p3);
            return;
        }

        QPointF mid = cubicBezierPoint(p0, p1, p2, p3, 0.5);
        QLineF chord(p0, p3);
        qreal deviation = QLineF(mid, chord.pointAt(0.5)).length();

        if (deviation > tolerance) {
            // 前半段控制点
            QPointF p01 = (p0 + p1) / 2;
            QPointF p12 = (p1 + p2) / 2;
            QPointF p23 = (p2 + p3) / 2;
            QPointF p012 = (p01 + p12) / 2;
            QPointF p123 = (p12 + p23) / 2;
            QPointF p0123 = (p012 + p123) / 2;

            subdivideBezier(p0, p01, p012, p0123, points, tolerance, depth - 1);
            subdivideBezier(p0123, p123, p23, p3, points, tolerance, depth - 1);
        } else {
            points.append(p3);
        }
    }

    inline QList<QPointF> discretizeWithBisection(const QPainterPath &path, qreal tolerance = 1.0,
                                                  int maxDepth = 4) {
        QList<QPointF> points;

        // 遍历路径元素
        for (int i = 0; i < path.elementCount();) {
            auto elem = path.elementAt(i);
            QPointF current(elem.x, elem.y);

            if (elem.isMoveTo() || elem.isLineTo()) {
                // 直线段处理（视为一次贝塞尔曲线）
                QPointF prev = (i > 0) ? path.elementAt(i - 1) : current;
                subdivideLine(prev, current, points, tolerance, maxDepth);
                i++;
            } else if (elem.type == QPainterPath::CurveToElement) {
                // 三次贝塞尔曲线处理
                QPointF p0 = path.elementAt(i - 1);
                QPointF p1 = path.elementAt(i);
                QPointF p2 = path.elementAt(i + 1);
                QPointF p3 = path.elementAt(i + 2);
                subdivideBezier(p0, p1, p2, p3, points, tolerance, maxDepth);
                i += 3;
            }
        }
        return points;
    }
}
#endif //COMMONFUNCTIONS_H
