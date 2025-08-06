//
// Created by unbd_ on 2025/5/8.
//

#include "CanvasConvertor.h"

BasicElement CanvasConvertor::convertToBasicElement(CanvasElement *canvasElement) {
    // 原数据
    auto elementId = canvasElement->getId();
    auto pluginName = canvasElement->getPluginName();
    auto properties = canvasElement->getProperties();
    auto points = canvasElement->getPoints();
    auto sharedPoints = canvasElement->getSelfLinkPoints();
    auto obverseLinkPoints = canvasElement->getObverseLinkPoints();

    // 转换为基本数据
    BasicElement element(pluginName, elementId);

    auto basicProperties = convertToBasicProperties(properties.get());
    auto basicPoints = QVector<BasicPoint>();
    auto basicSharedPoints = QVector<BasicPoint>();
    auto basicObverseLinkPoints = QVector<BasicPoint>();

    for (const auto &point: points) {
        basicPoints.push_back(convertToBasicPoint(&point));
    }

    for (const auto &point: sharedPoints) {
        if (const auto sharedPoint = point.toStrongRef()) {
            basicSharedPoints.push_back(convertToBasicPoint(sharedPoint.get()));
        }
    }

    for (const auto &point: obverseLinkPoints) {
        if (const auto sharedPoint = point.toStrongRef()) {
            basicObverseLinkPoints.push_back(convertToBasicPoint(sharedPoint.get()));
        }
    }

    element.points = basicPoints;
    element.sharedLinkPoints = basicSharedPoints;
    element.weakLinkPoints = basicObverseLinkPoints;
    element.properties = basicProperties;

    return element;
}

BasicPoint CanvasConvertor::convertToBasicPoint(const CanvasPointF *canvasPoint) {
    const auto pointId = canvasPoint->getId();
    auto point = BasicPoint(pointId, canvasPoint->x(), canvasPoint->y(), canvasPoint->getType());
    return point;
}

BasicProperty CanvasConvertor::convertToBasicProperty(const QVariant &property) {
    BasicProperty basicProperty;
    // 根据QVariant类型设置基本属性类型
    switch (property.userType()) {
        case QMetaType::QString:
            basicProperty.type = TypeDefine::BasicPropertyType::STRING;
            basicProperty.value = property.toString();
            break;
        case QMetaType::Int:
            basicProperty.type = TypeDefine::BasicPropertyType::INT;
            basicProperty.value = property.toString();
            break;
        case QMetaType::Double:
            basicProperty.type = TypeDefine::BasicPropertyType::FLOAT;
            basicProperty.value = property.toString();
            break;
        case QMetaType::Bool:
            basicProperty.type = TypeDefine::BasicPropertyType::BOOL;
            basicProperty.value = property.toBool() ? "true" : "false";
            break;
        case QMetaType::QColor: {
            const auto color = property.value<QColor>();
            basicProperty.type = TypeDefine::BasicPropertyType::COLOR;
            basicProperty.value = color.name(QColor::HexArgb);
            break;
        }
        default:
            // 处理枚举类型（需确保枚举已注册）
            if (property.canConvert<int>()) {
                basicProperty.type = TypeDefine::BasicPropertyType::ENUM;
                basicProperty.value = property.toInt();
            } else {
                throw std::runtime_error("Unsupported property type");
            }
    }
    return basicProperty;
}

QHash<QString, BasicProperty> CanvasConvertor::convertToBasicProperties(
    const PropertyType *propertyTypes
) {
    QHash<QString, BasicProperty> basicProperties;
    const auto rawProperties = propertyTypes->getProperties();
    for (const auto &key: rawProperties.keys()) {
        auto value = rawProperties.value(key);
        auto basicProperty = convertToBasicProperty(value);
        basicProperties.insert(key, basicProperty);
    }
    return basicProperties;
}

CanvasConvertor::CanvasConvertor(const QList<Tool *> &tools)
    : tools(tools) {
    for (auto tool: tools) {
        toolMap.insert(tool->name, tool);
    }
}

QList<CanvasElement *> CanvasConvertor::convertToCanvasElements(const QVector<BasicElement> &elements) {
    QList<CanvasElement *> canvasElements;
    QHash<QString, QSharedPointer<CanvasPointF> > pointMap;
    // 构造点映射表
    for (const auto &element: elements) {
        for (const auto &point: element.sharedLinkPoints) {
            CanvasPointF canvasPoint({point.x, point.y}, point.type, point.id);
            pointMap.insert(point.id, QSharedPointer<CanvasPointF>::create(canvasPoint));
        }
    }

    // 构造图元
    for (const auto &element: elements) {
        auto item = toolMap.value(element.pluginName)->itemDriver->newElement({});
        if (!item) continue;
        item->reserialize({
            element.pluginName,
            element.elementId,
            toolMap.value(element.pluginName)->type,
            convertToPropertyType(element.properties),
            convertToCanvasPointFs(element.points),
            getWeakPointer(element.weakLinkPoints, pointMap),
            getSharedPointer(element.sharedLinkPoints, pointMap),
        });
        canvasElements.push_back(item);
    }

    return canvasElements;
}

PropertyType CanvasConvertor::convertToPropertyType(const QHash<QString, BasicProperty> &properties) {
    PropertyType propertyType;
    for (const auto &key: properties.keys()) {
        const auto basicProperty = properties.value(key);
        auto finalValue = QVariant();
        switch (basicProperty.type) {
            case TypeDefine::BasicPropertyType::STRING:
                finalValue = basicProperty.value;
                break;
            case TypeDefine::BasicPropertyType::INT:
                finalValue = QVariant(basicProperty.value.toInt());
                break;
            case TypeDefine::BasicPropertyType::FLOAT:
                finalValue = QVariant(basicProperty.value.toDouble());
                break;
            case TypeDefine::BasicPropertyType::BOOL:
                finalValue = QVariant::fromValue<bool>(basicProperty.value == "true" ||
                                                       basicProperty.value == "1");
                break;
            case TypeDefine::BasicPropertyType::COLOR:
                finalValue = QVariant::fromValue<QColor>(basicProperty.value);
                break;
            case TypeDefine::BasicPropertyType::ENUM:
                finalValue = QVariant::fromValue<int>(static_cast<int>(basicProperty.value.toInt()));
                break;
            default:
                finalValue = QVariant(); // 未知类型设为空
        }
        propertyType.setProperty(key, finalValue);
    }
    return propertyType;
}

QList<QWeakPointer<CanvasPointF> > CanvasConvertor::getWeakPointer(const QVector<BasicPoint> &points,
                                                                   const QHash<QString, QSharedPointer<CanvasPointF> >
                                                                   &pointMap) {
    QList<QWeakPointer<CanvasPointF> > weakPointers;
    for (auto &point: points) {
        if (auto sharedPtr = pointMap.value(point.id)) {
            weakPointers.push_back(sharedPtr);
        } else {
            qWarning() << "Point ID not found in pointMap:" << point.id;
        }
    }
    return weakPointers;
}

QList<QSharedPointer<CanvasPointF> > CanvasConvertor::getSharedPointer(const QVector<BasicPoint> &points,
                                                                       const QHash<QString, QSharedPointer<
                                                                           CanvasPointF> >
                                                                       &pointMap) {
    QList<QSharedPointer<CanvasPointF> > sharedPointers;
    for (auto &point: points) {
        if (auto sharedPtr = pointMap.value(point.id)) {
            sharedPointers.push_back(sharedPtr);
        } else {
            qWarning() << "Point ID not found in pointMap:" << point.id;
        }
    }
    return sharedPointers;
}

QList<CanvasPointF> CanvasConvertor::convertToCanvasPointFs(const QVector<BasicPoint> &points) {
    QList<CanvasPointF> canvasPoints;
    for (const auto &point: points) {
        canvasPoints.push_back(convertToCanvasPointF(point));
    }
    return canvasPoints;
}

CanvasPointF CanvasConvertor::convertToCanvasPointF(const BasicPoint &point) {
    return CanvasPointF({point.x, point.y}, point.type, point.id);
}
