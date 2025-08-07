#pragma once

#include <QApplication>
#include <SubSystemFrame.h>
#include "FMainWindow.h"

class FApplication
    : public QApplication,
      public IGlobalSystem
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