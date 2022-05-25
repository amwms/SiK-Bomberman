#ifndef SIK_BOMBERMAN_CLIENTGAMESTATE_H
#define SIK_BOMBERMAN_CLIENTGAMESTATE_H

#include <memory>
#include <set>
#include "../utils/serializer.h"
#include "../utils/utils.h"

class ClientGameState {
public:
    bool in_lobby = true;

    StringSerializer server_name;
    UINT8Serializer players_count;
    UINT16Serializer size_x;
    UINT16Serializer size_y;
    UINT16Serializer game_length;
    UINT16Serializer explosion_radius;
    UINT16Serializer bomb_timer;

    MapSerializer<Player> players;

    UINT16Serializer turn;

    MapSerializer<Position> player_positions;
    std::set<Position> blocks;
    std::map<bomb_id_t, Bomb> bombs;
    std::set<Position> explosions;
    MapSerializer<score_t> scores;
    std::set<player_id_t> died_in_turn;

    ClientGameState() : server_name(""), players_count(0), size_x(0), size_y(0), game_length(0),
                        explosion_radius(0), bomb_timer(0), players(), turn(0), scores(), died_in_turn() {}
};

#endif //SIK_BOMBERMAN_CLIENTGAMESTATE_H
