#include "mainwindow.h"
#include <QMenuBar>
#include <QFile>
#include <QDebug>
#include <QApplication>
#include <QDir>
#include <QPluginLoader>
#include <QPushButton>
#include <QDialog>
#include <QColorDialog>
#include <QSettings>
#include <QFileDialog>
#include "IPlugin.h"
#include "../Convertor/CanvasConvertor.h"
#include "menus/NamedBar.h"
#include "QTextEdit"
#include "QVBoxLayout"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Flowchart Editor");
    resize(1920, 1080);
    setMinimumSize(1024, 768);

    // 画布
    canvasSpace = new CanvasSpace(this);
    setCentralWidget(canvasSpace);

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
    initCanvas();
    initPropertyEditor();

    updatePropertyTable();
    // 加载插件
    loadPlugin();

    reloadStyleSheet();
}

void MainWindow::reloadStyleSheet() {
    qDebug() << "reloadStyleSheet";
    QFile styleFile(R"(D:\code-projects-kdocs\FlowchartEditor\src\Main\style.css)");
    if (styleFile.open(QFile::ReadOnly)) {
        qApp->setStyleSheet(styleFile.readAll());
        styleFile.close();
    }
}

QHash<QString, QVariant> MainWindow::getProperties() const {
    return propManager->getProperties();
}

MainWindow::~MainWindow() = default;

void MainWindow::onPropertyChanged(const QString &key, const QVariant &value) {
    qDebug() << "Property updated:" << key << "=" << value;
    updatePropertyTable();
    propManager->apply();
}

void MainWindow::onCanvasItemSelected() {
    propManager->apply();
    propManager->setProperties(canvasSpace->getCurrentProperties());
    updatePropertyTable();
}

void MainWindow::initMenu() {
    // 菜单
    auto *openFileAction = new QAction(QString("open file"), this);
    auto *newfileAction = new QAction(QString("new file"), this);
    auto *saveFileAction = new QAction(QString("save"), this);
    auto *exportAction = new QAction(QString("export"), this);

    openFileAction->setIcon(QIcon(":/image/openfile_icon.svg"));
    newfileAction->setIcon(QIcon(":/image/newfile_icon.svg"));
    saveFileAction->setIcon(QIcon(":/image/save_icon.svg"));
    exportAction->setIcon(QIcon(":/image/export_icon.svg"));

    connect(openFileAction, &QAction::triggered, this, &MainWindow::loadFromFile);
    connect(saveFileAction, &QAction::triggered, this, &MainWindow::saveToFile);
    connect(newfileAction, &QAction::triggered, this, &MainWindow::resetCanvas);
    connect(exportAction, &QAction::triggered, this, &MainWindow::exportToImg);

    menus.push_back(openFileAction);
    menus.push_back(newfileAction);
    menus.push_back(saveFileAction);
    menus.push_back(exportAction);

    for (auto &menus: menus) {
        headBar->addAction(menus);
    }
}

void MainWindow::initTools() {
}

void MainWindow::initPropertyEditor() {
    // 创建停靠窗口和表格
    propDock = new QDockWidget(tr("Property Editor"), this);
    propTable = new QTableWidget(propDock);
    propTable->setColumnCount(2);
    propTable->setHorizontalHeaderLabels(QStringList() << "Property" << "value");
    propTable->horizontalHeader()->setStretchLastSection(true);

    // 创建容器控件和垂直布局
    QWidget *container = new QWidget(propDock);
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0); // 去除边距
    layout->addWidget(propTable);

    // 创建文本框并加入布局
    QTextEdit *helpText = new QTextEdit(container);
    helpText->setReadOnly(true); // 设为只读
    helpText->setHtml(QString(R"(
        <h2>Welcome</h2>
        <p>layer up: press page up</p>
        <p>layer down: press page down</p>
        <p>delete item: select and press delete</p>
        <p>move the canvas: use your mouse wheel. press ctrl and wheel for row scroll</p>)"));
    helpText->setFixedHeight(300); // 固定高度
    layout->addWidget(helpText);

    propDock->setWidget(container);
    propDock->setMinimumWidth(500);
    addDockWidget(Qt::RightDockWidgetArea, propDock);

    // 初始化属性管理器
    propManager = new PropertyManager(this);
    propManager->setProperties(canvasSpace->getCurrentProperties());
    connect(propManager, &PropertyManager::propertyChanged,
            this, &MainWindow::onPropertyChanged);
}

void MainWindow::updatePropertyTable() {
    propTable->clearContents();
    propTable->setRowCount(0);

    const auto &props = propManager->getProperties();
    for (auto it = props.begin(); it != props.end(); ++it) {
        int row = propTable->rowCount();
        propTable->insertRow(row);

        // 属性名列（不可编辑）
        QTableWidgetItem *nameItem = new QTableWidgetItem(it.key());
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
        propTable->setItem(row, 0, nameItem);

        // 值列（动态生成编辑器）
        QVariant value = it.value();
        if (value.type() == QVariant::Bool) {
            QComboBox *combo = new QComboBox();
            combo->addItems({"False", "True"});
            combo->setCurrentIndex(value.toBool() ? 1 : 0);
            connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                    [this, key = it.key()](int index) {
                        propManager->setProperty(key, index == 1);
                    });
            propTable->setCellWidget(row, 1, combo);
        } else if (value.type() == QVariant::Color) {
            // 创建颜色选择按钮
            QPushButton *colorBtn = new QPushButton();
            colorBtn->setAutoFillBackground(true);

            // 设置按钮背景色
            QColor color = value.value<QColor>();
            QString style = QString("background-color: %1; border: none;").arg(color.name());
            colorBtn->setStyleSheet(style);

            // 绑定点击事件
            connect(colorBtn, &QPushButton::clicked, [this, colorBtn, key = it.key()]() {
                QColorDialog dialog(colorBtn->palette().color(QPalette::Button));
                if (dialog.exec() == QDialog::Accepted) {
                    QColor selectedColor = dialog.selectedColor();
                    // 更新按钮颜色显示
                    colorBtn->setStyleSheet(
                        QString("background-color: %1; border: none;").arg(selectedColor.name()));
                    // 更新属性值
                    propManager->setProperty(key, selectedColor);
                }
            });
            propTable->setCellWidget(row, 1, colorBtn);
        } else {
            QTableWidgetItem *valueItem = new QTableWidgetItem(value.toString());
            propTable->setItem(row, 1, valueItem);
        }
    }

    // 绑定文本编辑信号
    connect(propTable, &QTableWidget::cellChanged, [this](const int row, const int col) {
        if (col == 1) {
            const QString key = propTable->item(row, 0)->text();
            const QVariant value = propTable->item(row, 1)->text();
            propManager->setProperty(key, value);
        }
    });
}

void MainWindow::loadPlugin() {
    QDir pluginsDir(qApp->applicationDirPath() + "/plugins");
    int i = 0;
    for (const QString &file: pluginsDir.entryList(QDir::Files)) {
        QPluginLoader loader(pluginsDir.absoluteFilePath(file));
        auto plugin = qobject_cast<IPlugin *>(loader.instance());
        if (!plugin)continue;

        plugin->Init();
        auto action = plugin->GetAction();
        auto itemType = new ItemDriver(
            std::bind(&IPlugin::createElement,
                      plugin,
                      std::placeholders::_1,
                      std::placeholders::_2) // 绑定参数
        );
        const auto tool = new Tool(plugin->GetPluginInfo().pluginName,
                                   plugin->GetToolType(), itemType);
        // 注册图形
        plugin->connectSignal([=]() {
            canvasSpace->selectTool(tool);
            action->setChecked(true);
        }, this);

        toolBar->addAction(action);
        canvasSpace->addTool(tool);
    }
}

void MainWindow::initCanvas() {
    auto cursorTool = new Tool("cursor", ToolType::SELECT);
    auto cursorAction = new QAction("choose tool", this);
    cursorAction->setIcon(QIcon(":/image/cursor_icon.svg"));

    toolBar->addAction(cursorAction);
    connect(cursorAction, &QAction::triggered, this, [=]() {
        canvasSpace->selectTool(cursorTool);
        qDebug() << "cursor tool action triggered";
    });
    canvasSpace->addTool(cursorTool);
    canvasSpace->selectTool(cursorTool);
    connect(canvasSpace, &CanvasSpace::itemSelected,
            this, &MainWindow::onCanvasItemSelected);
}

void MainWindow::saveToFile() {
    auto elements = QVector<BasicElement>();
    for (auto &element: canvasSpace->getCanvasElements()) {
        elements.append(CanvasConvertor::convertToBasicElement(element));
    }
    // 保存文件对话框
    QFileDialog saveFileDialog(this);
    saveFileDialog.setWindowTitle("Save File");
    saveFileDialog.setAcceptMode(QFileDialog::AcceptSave);
    saveFileDialog.setNameFilter("Flowchart Files (*.fc)");
    saveFileDialog.setDefaultSuffix("fc");
    saveFileDialog.setDirectory(QDir::currentPath());
    if (saveFileDialog.exec() == QDialog::Accepted) {
        QString fileName = saveFileDialog.selectedFiles().first();
        if (!fileName.endsWith(".fc")) {
            fileName += ".fc";
        }
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            QDataStream out(&file);
            out.setVersion(QDataStream::Qt_5_15); // 设置版本确保兼容性
            out << elements; // 直接序列化整个容器
            file.close();
        }
    }
}

void MainWindow::loadCanvasFromFile(const QString &chars) {
    QVector<BasicElement> elements;
    QFile file(chars);
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_15); // 必须与保存时版本一致
        in >> elements; // 自动反序列化整个容器
        file.close();
    }
    canvasSpace->loadFromData(elements);
}

void MainWindow::loadFromFile() {
    QFileDialog loadFileDialog(this);
    loadFileDialog.setWindowTitle("Load File");
    loadFileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    loadFileDialog.setNameFilter("Flowchart Files (*.fc)");
    loadFileDialog.setDirectory(QDir::currentPath());
    if (loadFileDialog.exec() == QDialog::Accepted) {
        QString fileName = loadFileDialog.selectedFiles().first();
        if (!fileName.endsWith(".fc")) {
            fileName += ".fc";
        }
        loadCanvasFromFile(fileName);
    }
}

void MainWindow::exportToImg() {
    QImage image = canvasSpace->getImage();
    QString fileName = QFileDialog::getSaveFileName(this, "Save Image", QDir::currentPath(),
                                                    "Images (*.png *.jpg *.bmp)");
    if (image.save(fileName)) {
    }
    qDebug() << "Image saved to" << fileName;
}

void MainWindow::resetCanvas() {
    canvasSpace->resetCanvas();
}
