#pragma once

#include "SubSystemFrame.h"
#include <string>
#include <qdebug.h>

class EPosition
    : public IComponent
{
public:
    int x = 0;
    int y = 0;
};

class EPlayer
    : public IComponent
{
public:
    std::string player_name = "default player";
    int player_id = 0;
};

class EPlayerEntity
    : public IEntity<EPosition, EPlayer>
{
};

class HandlePlayerMove
    : public ISubSystem<IQuerySingle<EPosition, EPlayer>>
{
    void execute(IQuerySingle<EPosition, EPlayer> &query) override;
};

class MovementSystem
    : public ISystem<HandlePlayerMove>
{
};