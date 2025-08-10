#pragma once

#include "ECSFrame.h"
#include <string>
#include <qdebug.h>

struct Velocity
{
    int vx = 0;
    int vy = 0;
};

class HandlePlayerMove
    : public ISubSystem<IQuerySingle<Velocity>>
{
    void execute(IQuerySingle<Velocity> &query) override
    {
        auto single = query.single();
        Velocity *velocity = single.getValue<Velocity>();
        velocity->vx += 1;
        velocity->vy += 1;
        qDebug() << "Player moved with velocity (" << velocity->vx << ", " << velocity->vy << ")";
        qDebug() << "Executing player move logic";
    }
};

void functionalSubSystem(IQuerySingle<Velocity> &query)
{
    auto single = query.single();
    Velocity *velocity = single.getValue<Velocity>();
    velocity->vx += 1;
    velocity->vy += 1;
    qDebug() << "Functional sub-system executed with velocity (" << velocity->vx << ", " << velocity->vy << ")";
}

void functionalSubSystem2(IQueryMul<Velocity> &query)
{
    // auto single = query.single();
    // Velocity *velocity = single.getValue<Velocity>();
    // velocity->vx += 1;
    // velocity->vy += 1;
    // qDebug() << "Functional sub-system executed with velocity (" << velocity->vx << ", " << velocity->vy << ")";
}

class MovementSystem
    : public ISystem<HandlePlayerMove>
{
    void main()
    {
        ISystemFunctionalSupport<functionalSubSystem, functionalSubSystem2> functionalSystem;
    }
};