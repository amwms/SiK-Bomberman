#ifndef SIK_BOMBERMAN_SERVERMESSAGE_H
#define SIK_BOMBERMAN_SERVERMESSAGE_H

#include "../serializer.h"
#include "../utils.h"
#include "../event.h"
#include <memory>

class ServerMessage {
protected:
    uint8_t message_id;

public:
    ServerMessage(uint8_t _message_id) : message_id(_message_id) {}
};

class HelloMessage : public ServerMessage {
    static const uint8_t HELLO_MESSAGE_ID = 0;

    StringSerializer server_name;
    UINT8Serializer players_count;
    UINT16Serializer size_x;
    UINT16Serializer size_y;
    UINT16Serializer game_length;
    UINT16Serializer explosion_radius;
    UINT16Serializer bomb_timer;

public:
    HelloMessage(const std::string &_server_name, const uint8_t _players_count,
                 const uint16_t _size_x, const uint16_t _size_y,
                 const uint16_t _game_length, const uint16_t _explosion_radius,
                 const uint16_t _bomb_timer) : ServerMessage(HELLO_MESSAGE_ID), server_name(_server_name),
                                                players_count(_players_count), size_x(_size_x),
                                                size_y(_size_y), game_length(_game_length),
                                                explosion_radius(_explosion_radius),
                                                bomb_timer(_bomb_timer) {}
};

class AcceptedPlayerMessage : public ServerMessage {
    static const uint8_t ACCEPTED_PLAYER_MESSAGE_ID = 1;

    player_id_t player_id;
    Player player;

public:
    AcceptedPlayerMessage(uint8_t _player_id, Player &_player) :
        ServerMessage(ACCEPTED_PLAYER_MESSAGE_ID),
        player_id(_player_id), player(_player) {}
};

class GameStartedMessage : public ServerMessage {
    static const uint8_t GAME_STARTED_MESSAGE_ID = 2;
    MapSerializer<Player> players;

public:
    GameStartedMessage(MapSerializer<Player> &_players) : ServerMessage(GAME_STARTED_MESSAGE_ID), players(_players) {}
};

class TurnMessage : public ServerMessage {
    static const uint8_t TURN_MESSAGE_ID = 3;

    UINT16Serializer turn;
    std::vector<std::shared_ptr<Event>> events;

public:
    TurnMessage(uint16_t _turn, std::vector<std::shared_ptr<Event>> &_events) :
        ServerMessage(TURN_MESSAGE_ID), turn(_turn), events(_events) {}
};

class GameEndedMessage : public ServerMessage {
    static const uint8_t GAME_ENDED_MESSAGE_ID = 4;
    MapSerializer<score_t> scores;

public:
    GameEndedMessage(MapSerializer<score_t> &_scores) : ServerMessage(GAME_ENDED_MESSAGE_ID), scores(_scores) {}
};

#endif //SIK_BOMBERMAN_SERVERMESSAGE_H
