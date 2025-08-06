//
// Created by unbd_ on 2025/5/3.
//

#ifndef HEADMENU_H
#define HEADMENU_H
#include "QToolBar"
#include <QWidget>

class HeadToolBar : public QToolBar {
    Q_OBJECT

public:
    HeadToolBar(QWidget *parent = nullptr): QToolBar(parent) {
        setObjectName(OBJECT_NAME);
        setMovable(false);
        setFloatable(false);
    }

private:
    QString OBJECT_NAME = "HeadMenu";
};

#endif //HEADMENU_H
