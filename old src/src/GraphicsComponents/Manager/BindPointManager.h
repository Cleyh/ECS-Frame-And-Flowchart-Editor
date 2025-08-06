#ifndef BINDPOINTMANAGER_H
#define BINDPOINTMANAGER_H
#include "QObject"
#include <QSharedPointer>
#include "../CanvasBase/CanvasElement.h"

class BindPointManager : public QObject {
    Q_OBJECT

public:
    explicit BindPointManager(QObject *parent = nullptr);
    ~BindPointManager() override;

    QSharedPointer<CanvasPointF> createLinkPoint(const QPointF &point);
    void destroyLinkPoint(const QSharedPointer<CanvasPointF> &point);

private:
    QList<QWeakPointer<CanvasPointF> > bindingPoints;
};


#endif //BINDPOINTMANAGER_H
