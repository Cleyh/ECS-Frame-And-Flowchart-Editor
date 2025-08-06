#ifndef BASEPOINT_H
#define BASEPOINT_H

#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <QHash>
#include "qdatastream.h"

#include "qdatastream.h"
#include "../../Utils/TypeDefine.h"


class BasicPoint {
public:
    QString id;
    float x = 0;
    float y = 0;
    TypeDefine::PointType type = TypeDefine::PointType::DEFAULT;
    BasicPoint() = default;

    explicit BasicPoint(const QString &id, const float x = 0, const float y = 0,
                        const TypeDefine::PointType type = TypeDefine::PointType::DEFAULT)
        : id(id), x(x), y(y), type(type) {
    }
};

inline QDataStream &operator<<(QDataStream &out, const BasicPoint &point) {
    out << point.id
            << point.x
            << point.y
            << static_cast<quint32>(point.type); // 枚举转整型存储
    return out;
}

inline QDataStream &operator>>(QDataStream &in, BasicPoint &point) {
    quint32 type;
    in >> point.id >> point.x >> point.y >> type;
    point.type = static_cast<TypeDefine::PointType>(type); // 整型转枚举
    return in;
}

class BasicProperty {
public:
    BasicProperty() = default;

    TypeDefine::BasicPropertyType type = TypeDefine::BasicPropertyType::STRING;
    QString value;
};

inline QDataStream &operator<<(QDataStream &out, const BasicProperty &prop) {
    out << static_cast<quint32>(prop.type) // 枚举转整型
            << prop.value;
    return out;
}

inline QDataStream &operator>>(QDataStream &in, BasicProperty &prop) {
    quint32 type;
    in >> type >> prop.value;
    prop.type = static_cast<TypeDefine::BasicPropertyType>(type); // 整型转枚举
    return in;
}

class BasicElement {
public:
    BasicElement() = default;

    BasicElement(const QString &pluginName, const QString &elementId)
        : pluginName(pluginName), elementId(elementId) { ; }

    QString pluginName;
    QString elementId;

    QVector<BasicPoint> points;
    QVector<BasicPoint> weakLinkPoints;
    QVector<BasicPoint> sharedLinkPoints;
    QHash<QString, BasicProperty> properties;
};

inline QDataStream &operator<<(QDataStream &out, const BasicElement &element) {
    out << element.pluginName
            << element.elementId
            << element.points // QVector自动处理元素个数
            << element.weakLinkPoints
            << element.sharedLinkPoints
            << element.properties; // QHash自动处理键值对
    return out;
}

inline QDataStream &operator>>(QDataStream &in, BasicElement &element) {
    in >> element.pluginName
            >> element.elementId
            >> element.points
            >> element.weakLinkPoints
            >> element.sharedLinkPoints
            >> element.properties;
    return in;
}

#endif //BASEPOINT_H
