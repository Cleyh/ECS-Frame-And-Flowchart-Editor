#pragma once

#include <QMainWindow>
#include <QWidget>

class FMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit FMainWindow(QWidget *parent = nullptr);
    ~FMainWindow() override;
};