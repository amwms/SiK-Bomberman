#ifndef SIK_BOMBERMAN_SERVERGAMESTATE_H
#define SIK_BOMBERMAN_SERVERGAMESTATE_H

#include <set>
#include <variant>
#include <shared_mutex>
#include "../utils/serializer.h"
#include "../utils/utils.h"
#include "../utils/messages/ClientMessage.h"
#include "../utils/messages/ServerMessage.h"
#include "../utils/RandomNumberGenerator.h"

class ServerGameState {
public:
    std::mutex mutex;
    bool is_lobby;

    // server game settings
    RandomNumberGenerator &randomizer;

    StringSerializer server_name;
    UINT8Serializer players_count;
    UINT16Serializer size_x;
    UINT16Serializer size_y;
    UINT16Serializer game_length;
    UINT16Serializer explosion_radius;
    UINT16Serializer bomb_timer;
    uint16_t initial_blocks_count;
    uint64_t turn_duration;
    // to be unchanged

    // current game memory
    uint8_t current_players_count = 0;
    MapSerializer<Player> players;
    std::set<Position> blocks;
    std::map<bomb_id_t, Bomb> bombs;
    MapSerializer<Position> player_positions;
    MapSerializer<score_t> scores;

    uint16_t turn_number = 0;
    std::vector<TurnMessage> game_turns;

    uint8_t current_player_id = 0;
    uint32_t current_bomb_id = 0;

    // to be reset every turn
    // information about changes in current turn
    std::set<Position> blocks_destroyed_in_turn;
    std::set<player_id_t> robots_destroyed_in_turn;

    ServerGameState(RandomNumberGenerator &_randomizer, const std::string &_server_name, uint8_t _players_count,
                    uint16_t _size_x, uint16_t _size_y, uint16_t _game_length, uint16_t _explosion_radius,
                    uint16_t _bomb_timer, uint16_t _initial_blocks_count, uint64_t _turn_duration) :
                                                      is_lobby(true),
                                                      randomizer(_randomizer),
                                                      server_name(_server_name),
                                                      players_count(_players_count),
                                                      size_x(_size_x), size_y(_size_y),
                                                      game_length(_game_length),
                                                      explosion_radius(_explosion_radius),
                                                      bomb_timer(_bomb_timer),
                                                      initial_blocks_count(_initial_blocks_count),
                                                      turn_duration(_turn_duration) {}

    std::vector<std::shared_ptr<Event>> initialize_new_game();

    void reset_game_state();

    player_id_t get_next_player_id();

    bomb_id_t get_next_bomb_id();

    void update_after_turn();

    void reset_turn_data();

    void update_bomb_timers();

    HelloMessage to_hello_message();
};


#endif //SIK_BOMBERMAN_SERVERGAMESTATE_H
