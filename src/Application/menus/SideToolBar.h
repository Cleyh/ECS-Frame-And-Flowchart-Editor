//
// Created by unbd_ on 2025/5/3.
//

#ifndef SIDETOOLBAR_H
#define SIDETOOLBAR_H
#include "QToolBar"

class SideToolBar : public QToolBar {
    Q_OBJECT
public:
    SideToolBar(QWidget *parent = nullptr)
        : QToolBar(parent) {
        setObjectName(OBJECT_NAME);
    }

private:
    QString OBJECT_NAME = "SideToolBar";
};

#endif //SIDETOOLBAR_H
