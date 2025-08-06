#include "FMainWindow.h"

FMainWindow::FMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Flowchart Editor");
    resize(1024, 768);
    setMinimumSize(1024, 768);
}

FMainWindow::~FMainWindow()
{
}
