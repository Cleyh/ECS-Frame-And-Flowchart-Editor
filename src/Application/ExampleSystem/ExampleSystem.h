#pragma once

#include "ECSFrame.h"
#include <string>
#include <qdebug.h>

struct Velocity
{
    int vx = 0;
    int vy = 0;
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
{
    void main()
    {
        ISystem system;
        system.addFunction(&functionalSubSystem);
    }
};