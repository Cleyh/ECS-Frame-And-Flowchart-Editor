#pragma once

#include <QApplication>
#include "FMainWindow.h"

class FApplication : public QApplication
{
    Q_OBJECT
public:
    explicit FApplication(int &argc, char **argv);
    ~FApplication() override;

    void initialize();
    void cleanup();

private:
    FMainWindow *mainWindow;
};