#pragma once

#include "SubSystemFrame.h"

class EPosition
    : public IComponent
{
public:
    EPosition(int x = 0, int y = 0) : x(x), y(y)
    {
    }

    int x;
    int y;

    bool operator==(const EPosition &other) const
    {
        return x == other.x && y == other.y;
    }
};

class EPlayer
    : public IComponent
{
};

class EPlayerEntity
    : public IEntity<EPosition, EPlayer>
{
};

class MovementSystem
    : public ISubSystem
{
    void execute() override {
    }
};

class EPositionSystem
    : public ISystem<MovementSystem>
{
};