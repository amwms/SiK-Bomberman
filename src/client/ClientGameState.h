#ifndef SIK_BOMBERMAN_CLIENTGAMESTATE_H
#define SIK_BOMBERMAN_CLIENTGAMESTATE_H

#include <memory>
#include <set>
#include "../utils/serializer.h"
#include "../utils/utils.h"
#include "../utils/messages/DrawMessage.h"

class ClientGameState {
public:
    bool in_lobby = true;
    bool should_send_update_to_gui = false;

    StringSerializer server_name;
    UINT8Serializer players_count;
    UINT16Serializer size_x;
    UINT16Serializer size_y;
    UINT16Serializer game_length;
    UINT16Serializer explosion_radius;
    UINT16Serializer bomb_timer;

    UINT16Serializer turn;

    MapSerializer<Player> players;
    std::set<Position> blocks;
    std::map<bomb_id_t, Bomb> bombs;
    MapSerializer<Position> player_positions;
    MapSerializer<score_t> scores;

    // to be reset every turn
    // information about changes in current turn
    std::set<Position> blocks_destroyed_in_turn;
    std::set<Position> explosions;
    std::set<player_id_t> robots_destroyed_in_turn;

    ClientGameState() : server_name(""), players_count(0), size_x(0), size_y(0), game_length(0),
                        explosion_radius(0), bomb_timer(0), turn(0), scores() {}

    void update_after_turn();

    void reset_turn_data();

    LobbyMessage to_lobby_massage();

    GameMessage to_game_message();
};

#endif //SIK_BOMBERMAN_CLIENTGAMESTATE_H
