#pragma once
#include <QMainWindow>
#include <iostream>
#include <vector>
#include <QTableWidget>
#include <QDockWidget>
#include <QComboBox>
#include <QHeaderView>
#include <QVariant>
#include "CanvasSpace.h"
#include "PropertyManager/PropertyManager.h"
#include "Basic/BasicPoint.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void reloadStyleSheet();
    QHash<QString, QVariant> getProperties() const;
    ~MainWindow() override;

private slots:
    void onPropertyChanged(const QString &key, const QVariant &value);
    void onCanvasItemSelected();

private:
    void initMenu();
    void initTools();
    void initPropertyEditor();
    void updatePropertyTable();

    void loadPlugin();
    void initCanvas();

public:
    void saveToFile();
    void loadCanvasFromFile(const QString & chars);
    void loadFromFile();
    void exportToImg();
    void resetCanvas();

private:
    PropertyManager *propManager;
    QDockWidget *propDock;
    QTableWidget *propTable;

    QToolBar *headBar;
    QToolBar *toolBar;
    CanvasSpace *canvasSpace;
    std::vector<QAction *> menus;
};
