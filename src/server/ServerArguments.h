#ifndef SIK_BOMBERMAN_SERVERARGUMENTS_H
#define SIK_BOMBERMAN_SERVERARGUMENTS_H

#include <string>

class ServerArguments {
    uint16_t bomb_timer;
    uint8_t players_count;
    uint64_t turn_duration;
    uint16_t explosion_radius;
    uint16_t initial_blocks;
    uint16_t game_length;
    std::string server_name;
    uint16_t port;
    uint32_t seed;
    uint16_t size_x;
    uint16_t size_y;

public:
    ServerArguments(uint16_t _bomb_timer, uint8_t _players_count, uint64_t _turn_duration,
                    uint16_t _explosion_radius, uint16_t _initial_blocks, uint16_t _game_length,
                    const std::string &serverName, uint16_t port, uint32_t seed, uint16_t _size_x,
                    uint16_t _size_y) : bomb_timer(_bomb_timer), players_count(_players_count),
                                        turn_duration(_turn_duration), explosion_radius(_explosion_radius),
                                        initial_blocks(_initial_blocks), game_length(_game_length),
                                        server_name(serverName), port(port), seed(seed), size_x(_size_x),
                                        size_y(_size_y) {}
};

ServerArguments parse_server_arguments(int argc, char *argv[]);

#endif //SIK_BOMBERMAN_SERVERARGUMENTS_H
