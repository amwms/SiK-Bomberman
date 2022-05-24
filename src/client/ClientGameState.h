#ifndef SIK_BOMBERMAN_CLIENTGAMESTATE_H
#define SIK_BOMBERMAN_CLIENTGAMESTATE_H

#include <memory>
#include <set>
#include "../utils/serializer.h"
#include "../utils/utils.h"
#include "../utils/event.h"

class ClientGameState {
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
    std::vector<std::shared_ptr<Event>> events;

    MapSerializer<score_t> scores;

    std::set<player_id_t> died_in_turn;

public:
    ClientGameState() : server_name(""), players_count(0), size_x(0), size_y(0), game_length(0),
                        explosion_radius(0), bomb_timer(0), players(), turn(0),
                        events(0), scores(), died_in_turn() {}

    void set_server_name(const StringSerializer &_server_name) {
        server_name = _server_name;
    }

    void set_players_count(const UINT8Serializer &_players_count) {
        players_count = _players_count;
    }

    void set_size_x(const UINT16Serializer &_size_x) {
        size_x = _size_x;
    }

    void set_size_y(const UINT16Serializer &_size_y) {
        size_y = _size_y;
    }

    void set_game_length(const UINT16Serializer &_game_length) {
        game_length = _game_length;
    }

    void set_explosion_radius(const UINT16Serializer &_explosion_radius) {
        explosion_radius = _explosion_radius;
    }

    void set_bomb_timer(const UINT16Serializer &_bomb_timer) {
        bomb_timer = _bomb_timer;
    }

    void add_players(const player_id_t &_player_id, const Player &_player) {
        players.get_map().insert(std::make_pair(_player_id.get_num(), _player));
    }

    void set_players(const MapSerializer<Player> &_players) {
        ClientGameState::players = _players;
    }

    void set_turn(const UINT16Serializer &_turn) {
        ClientGameState::turn = _turn;
    }

    void set_events(const std::vector<std::shared_ptr<Event>> &_events) {
        ClientGameState::events = _events;
    }

    void set_scores(const MapSerializer<score_t> &_scores) {
        ClientGameState::scores = _scores;
    }

    void add_died_in_turn(const player_id_t &_player_id) {
        died_in_turn.insert(_player_id);
    }

    void set_is_in_lobby(bool _is_in_lobby) {
        ClientGameState::in_lobby = _is_in_lobby;
    }

    bool is_in_lobby() const {
        return in_lobby;
    }
};

#endif //SIK_BOMBERMAN_CLIENTGAMESTATE_H
