#pragma once

#include <qmainwindow.h>
#include <qwidget.h>
#include <qvector.h>

class FMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit FMainWindow(QWidget *parent = nullptr);
    ~FMainWindow() override;

protected:
    void initMenu();
    void initTools();
    void reloadStyleSheet();

public:
    void loadFromFile();
    void saveToFile();
    void resetCanvas();
    void exportToImg();

private:
    QToolBar *headBar;
    QToolBar *toolBar;
    QVector<QAction *> menus;
};