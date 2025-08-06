//
// Created by unbd_ on 2025/5/8.
//

#ifndef CANVASCONVERTOR_H
#define CANVASCONVERTOR_H
#include "CanvasSpace.h"
#include "Basic/BasicPoint.h"
#include "CanvasBase/CanvasElement.h"
#include "qlist.h"
#include "qstring.h"
#include "qhash.h"
#include "qvariant.h"
#include "qsharedpointer.h"

class Tool;

class CanvasConvertor {
public:
    static BasicElement convertToBasicElement(CanvasElement *canvasElement);
    static BasicPoint convertToBasicPoint(const CanvasPointF *canvasPoint);
    static BasicProperty convertToBasicProperty(const QVariant &property);
    static QHash<QString, BasicProperty> convertToBasicProperties(const PropertyType *PropertyTypes);

public:
    explicit CanvasConvertor(const QList<Tool *> &tools);
    QList<CanvasElement *> convertToCanvasElements(const QVector<BasicElement> &elements);
    PropertyType convertToPropertyType(const QHash<QString, BasicProperty> &properties);
    QList<QWeakPointer<CanvasPointF> > getWeakPointer(const QVector<BasicPoint> &points,
                                                      const QHash<QString, QSharedPointer<CanvasPointF> > &pointMap);
    QList<QSharedPointer<CanvasPointF> > getSharedPointer(const QVector<BasicPoint> &points,
                                                          const QHash<QString, QSharedPointer<CanvasPointF> > &
                                                          pointMap);
    QList<CanvasPointF> convertToCanvasPointFs(const QVector<BasicPoint> &points);
    CanvasPointF convertToCanvasPointF(const BasicPoint &point);

private:
    QHash<QString, Tool *> toolMap;
    const QList<Tool *> tools;
};


#endif //CANVASCONVERTOR_H
