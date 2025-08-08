#pragma once

#include "SubSystemFrame.h"
#include <string>
#include <qdebug.h>

// class EPosition
//     : public IComponent
// {
// public:
//     int x = 0;
//     int y = 0;
// };

// class EPlayer
//     : public IComponent
// {
// public:
//     std::string player_name = "default player";
//     int player_id = 0;
// };

// class EPlayerEntity
//     : public IEntity<EPosition, EPlayer>
// {
// };

// class HandlePlayerMove
//     : public ISubSystem<IQuerySingle<EPosition, EPlayer>>
// {
//     void execute(IQuerySingle<EPosition, EPlayer> &query) override;
// };

// class MovementSystem
//     : public ISystem<HandlePlayerMove>
// {
// };

struct Velocity
{
    int vx = 0;
    int vy = 0;
};

// class ECarEntity
//     : public IEntity<Velocity>
// {
// };

class HandlePlayerMove
    : public ISubSystem<IQuerySingle<Velocity>>
{
    void execute(IQuerySingle<Velocity> &query) override{
        auto single = query.single();
        Velocity *velocity = single.getValue<Velocity>();
        velocity->vx += 1;
        velocity->vy += 1;
        qDebug() << "Player moved with velocity (" << velocity->vx << ", " << velocity->vy << ")";
        qDebug() << "Executing player move logic";
    }
};

class MovementSystem
    : public ISystem<HandlePlayerMove>
{
};