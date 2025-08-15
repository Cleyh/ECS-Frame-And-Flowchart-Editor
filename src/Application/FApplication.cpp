#include <qdebug.h>

#include "FApplication.h"
#include "ExampleSystem/ExampleSystem.h"

FApplication::FApplication(int &argc, char **argv)
    : QApplication(argc, argv)
{
    initialize();
}

FApplication::~FApplication()
{
    cleanup();
}

void FApplication::initialize()
{
    mainWindow = new FMainWindow();
    mainWindow->show();

    IEntity<Velocity> playerEntity;
    IEntity<Velocity> playerEntity2;

    ISystem system;
    system.addFunction(functionalSubSystem);

    IGlobalSystem::getInstance()->addSystem(&system);
    IGlobalSystem::getInstance()->addEntity(&playerEntity);

    IGlobalSystem::getInstance()->notify();
    IGlobalSystem::getInstance()->notify();
    
    IGlobalSystem::getInstance()->addEntity(&playerEntity2);
    IGlobalSystem::getInstance()->notify();
    qDebug() << "FApplication initialized with main window and systems.";
}

void FApplication::cleanup()
{
    if (mainWindow)
    {
        delete mainWindow;
        mainWindow = nullptr;
    }
}

bool FApplication::notify(QObject *qobject, QEvent *qevent)
{
    return QApplication::notify(qobject, qevent);
}
