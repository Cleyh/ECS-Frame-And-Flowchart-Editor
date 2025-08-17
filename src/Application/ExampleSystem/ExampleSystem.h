#pragma once

#include "ECSFrame.h"
#include <string>
#include <qdebug.h>

struct Velocity
{
    int vx = 0;
    int vy = 0;
};

struct Position
{
    int x = 0;
    int y = 0;
};

void functionalSubSystem(IQueryMul<Velocity> &query)
{
    auto entities = query.mul();
    for (auto single : entities)
    {
        // Assuming single is an IEntity<Velocity>
        // and has a method getValue to retrieve the Velocity component.
        auto velocity = single.getValue<Velocity>();
        velocity->vx += 1;
        velocity->vy += 1;
        qDebug() << "entity: " << single.getInstanceId() << "Functional sub-system executed with velocity (" << velocity->vx << ", " << velocity->vy << ")";
    }
}

void functionalSubSystem2(IQueryMul<Velocity> query_no_ref)
{
    // auto single = query.single();
    // Velocity *velocity = single.getValue<Velocity>();
    // velocity->vx += 1;
    // velocity->vy += 1;
    // qDebug() << "Functional sub-system executed with velocity (" << velocity->vx << ", " << velocity->vy << ")";
}