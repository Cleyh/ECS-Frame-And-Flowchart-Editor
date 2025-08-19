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

void functionalSubSystem(QueryMul<Velocity> &query)
{
    auto entities = query.mul();
    for (auto &entity : entities)
    {
        auto velocity = entity->Value<Velocity>();
        velocity->vx += 1;
        velocity->vy += 1;

        qDebug() << "Velocity updated: "
                 << "vx = " << velocity->vx
                 << ", vy = " << velocity->vy;
    }
}