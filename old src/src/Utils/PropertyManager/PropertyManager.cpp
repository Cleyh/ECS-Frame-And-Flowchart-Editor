#include "PropertyManager.h"
#include <qdebug.h>

PropertyManager::PropertyManager(QObject *parent)
    : QObject(parent) { ; }

void PropertyManager::setProperties(const std::shared_ptr<PropertyType> &prop) {
    properties = prop;
    if (prop) propsCopy = prop->getProperties();
    else propsCopy.clear();
}


QHash<QString, QVariant> PropertyManager::getProperties() const {
    return propsCopy;
}

void PropertyManager::setProperty(const QString &key, const QVariant &value) {
    if (propsCopy[key] != value) {
        propsCopy[key] = value;
        qDebug() << propsCopy[key].toString();
        emit propertyChanged(key, value);
    }
}

QVariant PropertyManager::getProperty(const QString &key) const {
    return propsCopy[key];
}

void PropertyManager::apply() {
    if (auto shared_prop = properties.lock()) {
        qDebug() << "PropertyManager::apply(): properties are applied";
        shared_prop->replaceProperties(propsCopy);
    } else {
        qWarning() << "PropertyManager::apply(): properties could not be retrieved";
        // emit propertyChanged("", QVariant());
        propsCopy.clear();
    }
}
