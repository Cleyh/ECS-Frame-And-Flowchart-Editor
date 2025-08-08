#include "ExampleSystem.h"

// void HandlePlayerMove::execute(IQuerySingle<EPosition, EPlayer> &query)
// {
//     auto single = query.single();
//     EPosition *pos = single.getValue<EPosition>();
//     EPlayer *player = single.getValue<EPlayer>();

//     pos->x += 1;
//     pos->y += 1;
//     player->player_id += 1;
//     qDebug() << "Player moved to (" << pos->x << ", " << pos->y << ")"
//              << "Player ID: " << player->player_id
//              << "Player Name: " << QString::fromStdString(player->player_name);
// }