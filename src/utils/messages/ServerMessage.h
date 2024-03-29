#ifndef SIK_BOMBERMAN_SERVERMESSAGE_H
#define SIK_BOMBERMAN_SERVERMESSAGE_H

#include "../serializer.h"
#include "../utils.h"
#include "../event.h"
#include <memory>

class ServerMessage : public Serializer {
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

    HelloMessage(StringSerializer &_server_name,
                               UINT8Serializer &_players_count,
                               UINT16Serializer &_size_x, UINT16Serializer &_size_y,
                               UINT16Serializer &_game_length, UINT16Serializer &_explosion_radius,
                               UINT16Serializer &_bomb_timer) :
                               ServerMessage(HELLO_MESSAGE_ID), server_name(_server_name),
                                                                    players_count(_players_count), size_x(_size_x),
                                                                    size_y(_size_y), game_length(_game_length),
                                                                    explosion_radius(_explosion_radius),
                                                                    bomb_timer(_bomb_timer) {}

    std::string serialize() const override;

    const StringSerializer &get_server_name() const;

    const UINT8Serializer &get_players_count() const;

    const UINT16Serializer &get_size_x() const;

    const UINT16Serializer &get_size_y() const;

    const UINT16Serializer &get_game_length() const;

    const UINT16Serializer &get_explosion_radius() const;

    const UINT16Serializer &get_bomb_timer() const;
};

class AcceptedPlayerMessage : public ServerMessage {
    static const uint8_t ACCEPTED_PLAYER_MESSAGE_ID = 1;

    player_id_t player_id;
    Player player;

public:
    AcceptedPlayerMessage(uint8_t _player_id, Player &_player) :
        ServerMessage(ACCEPTED_PLAYER_MESSAGE_ID),
        player_id(_player_id), player(_player) {}

    AcceptedPlayerMessage(player_id_t &_player_id, Player &_player) :
            ServerMessage(ACCEPTED_PLAYER_MESSAGE_ID),
            player_id(_player_id), player(_player) {}

    std::string serialize() const override;

    const player_id_t &get_player_id() const;

    const Player &get_player() const;
};

class GameStartedMessage : public ServerMessage {
    static const uint8_t GAME_STARTED_MESSAGE_ID = 2;
    MapSerializer<Player> players;

public:
    GameStartedMessage(MapSerializer<Player> &_players) : ServerMessage(GAME_STARTED_MESSAGE_ID), players(_players) {}

    std::string serialize() const override;

    MapSerializer<Player> &get_players();
};

class TurnMessage : public ServerMessage {
    static const uint8_t TURN_MESSAGE_ID = 3;

    UINT16Serializer turn;
    std::vector<std::shared_ptr<Event>> events;

public:
    TurnMessage(uint16_t _turn, std::vector<std::shared_ptr<Event>> &_events) :
        ServerMessage(TURN_MESSAGE_ID), turn(_turn), events(_events) {}

    TurnMessage(UINT16Serializer &_turn, std::vector<std::shared_ptr<Event>> &_events) :
            ServerMessage(TURN_MESSAGE_ID), turn(_turn), events(_events) {}

    std::string serialize() const override;

    UINT16Serializer &get_turn();

    std::vector<std::shared_ptr<Event>> &get_events();
};

class GameEndedMessage : public ServerMessage {
    static const uint8_t GAME_ENDED_MESSAGE_ID = 4;
    MapSerializer<score_t> scores;

public:
    GameEndedMessage(MapSerializer<score_t> &_scores) : ServerMessage(GAME_ENDED_MESSAGE_ID), scores(_scores) {}

    std::string serialize() const override;

    MapSerializer<score_t> &get_scores();
};

#endif //SIK_BOMBERMAN_SERVERMESSAGE_H
