#include "FMainWindow.h"

#include <qdebug.h>
#include <QFile>
#include <QFileDialog>

#include "menus/NamedBar.h"

FMainWindow::FMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Flowchart Editor");
    resize(1024, 768);
    setMinimumSize(1024, 768);

    // 菜单栏
    headBar = new NamedBar("HeadMenu", "HeadMenuItem", this);
    headBar->setMovable(false);
    headBar->setFloatable(false);
    addToolBar(Qt::TopToolBarArea, headBar);

    // 工具栏
    toolBar = new NamedBar("SideToolBar", "SideToolBarItem", this);
    toolBar->setOrientation(Qt::Vertical);
    addToolBar(Qt::LeftToolBarArea, toolBar);

    initMenu();
    initTools();
    reloadStyleSheet();
}

FMainWindow::~FMainWindow()
{
}

void FMainWindow::initMenu() {
    // 菜单
    auto *openFileAction = new QAction(QString("open file"), this);
    auto *newfileAction = new QAction(QString("new file"), this);
    auto *saveFileAction = new QAction(QString("save"), this);
    auto *exportAction = new QAction(QString("export"), this);

    openFileAction->setIcon(QIcon(":/image/openfile_icon.svg"));
    newfileAction->setIcon(QIcon(":/image/newfile_icon.svg"));
    saveFileAction->setIcon(QIcon(":/image/save_icon.svg"));
    exportAction->setIcon(QIcon(":/image/export_icon.svg"));

    connect(openFileAction, &QAction::triggered, this, &FMainWindow::loadFromFile);
    connect(saveFileAction, &QAction::triggered, this, &FMainWindow::saveToFile);
    connect(newfileAction, &QAction::triggered, this, &FMainWindow::resetCanvas);
    connect(exportAction, &QAction::triggered, this, &FMainWindow::exportToImg);

    menus.push_back(openFileAction);
    menus.push_back(newfileAction);
    menus.push_back(saveFileAction);
    menus.push_back(exportAction);

    for (auto &menus: menus) {
        headBar->addAction(menus);
    }
}

void FMainWindow::initTools() {
}

void FMainWindow::reloadStyleSheet() {
    qDebug() << "reloadStyleSheet";
    QFile styleFile(R"(D:\code-projects-kdocs\FlowchartEditor\src\Main\style.css)");
    if (styleFile.open(QFile::ReadOnly)) {
        setStyleSheet(styleFile.readAll());
        styleFile.close();
    }
}

void FMainWindow::loadFromFile() {
    // Load from file logic
    qDebug() << "Loading from file.";
}

void FMainWindow::saveToFile() {
    // Save to file logic
    qDebug() << "Saving to file.";
}

void FMainWindow::resetCanvas() {
    // Reset canvas logic
    qDebug() << "Canvas reset.";
}

void FMainWindow::exportToImg() {
    // Export canvas to image logic
    qDebug() << "Exporting canvas to image.";
}