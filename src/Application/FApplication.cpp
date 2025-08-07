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

    IEntity<EPosition, EPlayer> playerEntity;
    MovementSystem movementSystem;
    
    IGlobalSystem::getInstance()->addSystem(&movementSystem);
    IGlobalSystem::getInstance()->notify();
    qDebug() << "FApplication initialized with main window and systems.";

}

void FApplication::cleanup()
{
    if (mainWindow) {
        delete mainWindow;
        mainWindow = nullptr;
    }
}