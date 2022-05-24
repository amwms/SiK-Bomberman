#ifndef SIK_BOMBERMAN_DRAWMESSAGE_H
#define SIK_BOMBERMAN_DRAWMESSAGE_H

#include "../serializer.h"
#include "../utils.h"

class DrawMessage : public Serializer {
protected:
    uint8_t message_id;

public:
    explicit DrawMessage(uint8_t _message_id) : message_id(_message_id) {}
};

class LobbyMessage : public DrawMessage {
    static const uint8_t LOBBY_MESSAGE_ID = 0;

    StringSerializer server_name;
    uint8_t players_count;
    UINT16Serializer size_x;
    UINT16Serializer size_y;
    UINT16Serializer game_length;
    UINT16Serializer explosion_radius;
    UINT16Serializer bomb_timer;
    MapSerializer<Player> players;

public:
    LobbyMessage(const std::string &_server_name, uint8_t _players_count, uint16_t _size_x,
                 uint16_t _size_y, uint16_t _game_length, uint16_t _explosion_radius, uint16_t _bomb_timer,
                 const MapSerializer<Player> &players) : DrawMessage(LOBBY_MESSAGE_ID), server_name(_server_name),
                                                         players_count(_players_count), size_x(_size_x),
                                                         size_y(_size_y), game_length(_game_length),
                                                         explosion_radius(_explosion_radius),
                                                         bomb_timer(_bomb_timer), players(players) {}

    std::string serialize() const override;
};

class GameMessage : public DrawMessage {
    static const uint8_t GAME_MESSAGE_ID = 1;

    StringSerializer server_name;
    UINT16Serializer size_x;
    UINT16Serializer size_y;
    UINT16Serializer game_length;
    UINT16Serializer turn;
    MapSerializer<Player> players;
    MapSerializer<Position> player_positions;
    ListSerializer<Position> blocks;
    ListSerializer<Bomb> bombs;
    ListSerializer<Position> explosions;
    MapSerializer<score_t> scores;

public:
    GameMessage(const std::string &_server_name, uint16_t _size_x,
                uint16_t _size_y, uint16_t _game_length,
                uint16_t turn, const MapSerializer<Player> &players,
                const MapSerializer<Position> &_player_positions, const ListSerializer<Position> &blocks,
                const ListSerializer<Bomb> &bombs, const ListSerializer<Position> &explosions,
                const MapSerializer<score_t> &scores) : DrawMessage(GAME_MESSAGE_ID), server_name(_server_name),
                                                        size_x(_size_x), size_y(_size_y), game_length(_game_length),
                                                        turn(turn), players(players),
                                                        player_positions(_player_positions), blocks(blocks),
                                                        bombs(bombs), explosions(explosions), scores(scores) {}

    std::string serialize() const override;
};

#endif //SIK_BOMBERMAN_DRAWMESSAGE_H
