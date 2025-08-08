#include <iostream>
#include "FApplication.h"
#include <qdebug.h>

class A
{
};

class B
{
};

int main(int argc, char *argv[])
{
    // 输出欢迎信息
    std::cout << "Welcome to FlowchartEditor!" << std::endl;
    FApplication app(argc, argv);
    return app.exec();
}