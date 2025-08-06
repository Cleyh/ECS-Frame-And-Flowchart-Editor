#pragma once
#include <vector>
#include "ISubSystemObject.h"

class ISubSystem;
typedef std::vector<ISubSystem*> SubSystemList;

class ISubSystem
{
public:
    virtual void execute() = 0; // Pure virtual function to be implemented by derived subsystems
private:
    SubSystemList subSystemList; // List of subsystems
};