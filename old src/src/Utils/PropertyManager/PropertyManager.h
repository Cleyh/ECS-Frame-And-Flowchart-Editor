// PropertyManager.h
#ifndef PROPERTYMANAGER_H
#define PROPERTYMANAGER_H
#include <QObject>
#include <QHash>
#include <QVariant>
#include <QSharedPointer>

class PropertyType{
public:
    virtual ~PropertyType() = default;
    virtual QVariant getProperty(const QString &key) const { return properties.value(key); }
    virtual QHash<QString, QVariant> getProperties() const { return properties; }

    virtual void replaceProperties(const QHash<QString, QVariant> &props) {
        properties = props;
    }

    virtual void setProperty(const QString &key, const QVariant &value) {
        if (properties[key] != value) {
            properties[key] = value;
        }
    }

protected:
    QHash<QString, QVariant> properties;
};

/**
 * 属性管理器
 */
class PropertyManager : public QObject {
    Q_OBJECT

public:
    explicit PropertyManager(QObject *parent = nullptr);

    void setProperties(const std::shared_ptr<PropertyType> &prop);
    QHash<QString, QVariant> getProperties() const;
    void setProperty(const QString &key, const QVariant &value);
    QVariant getProperty(const QString &key) const;
    void apply();

signals:
    void propertyChanged(const QString &key, const QVariant &value);

private:
    QHash<QString, QVariant> propsCopy;
    std::weak_ptr<PropertyType> properties;
};


#endif //PROPERTYMANAGER_H
