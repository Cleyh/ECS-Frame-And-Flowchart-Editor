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
}

void FApplication::cleanup()
{
    if (mainWindow) {
        delete mainWindow;
        mainWindow = nullptr;
    }
}