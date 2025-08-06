//
// Created by unbd_ on 2025/5/3.
//

#ifndef NAMEDBAR_H
#define NAMEDBAR_H

#include <QToolBar>

class NamedBar : public QToolBar {
    Q_OBJECT

public:
    NamedBar(QString thisName, QString itemName, QWidget *parent = nullptr)
        : QToolBar(parent),
          OBJECT_NAME(thisName),
          SUBITEM_NAME(itemName) {
        setObjectName(OBJECT_NAME);
    }

    void addAction(QAction *action) {
        action->setObjectName(SUBITEM_NAME);
        QToolBar::addAction(action);
    }

private:
    QString OBJECT_NAME;
    QString SUBITEM_NAME;
};

#endif //NAMEDBAR_H
