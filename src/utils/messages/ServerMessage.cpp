#include "ServerMessage.h"

std::string HelloMessage::serialize() {
    std::string result{static_cast<char>(message_id)};
    result += server_name.serialize();
    result += players_count.serialize();
    result += size_x.serialize();
    result += size_y.serialize();
    result += game_length.serialize();
    result += explosion_radius.serialize();
    result += bomb_timer.serialize();

    return result;
}

const StringSerializer &HelloMessage::get_server_name() const {
    return server_name;
}

const UINT8Serializer &HelloMessage::get_players_count() const {
    return players_count;
}

const UINT16Serializer &HelloMessage::get_size_x() const {
    return size_x;
}

const UINT16Serializer &HelloMessage::get_size_y() const {
    return size_y;
}

const UINT16Serializer &HelloMessage::get_game_length() const {
    return game_length;
}

const UINT16Serializer &HelloMessage::get_explosion_radius() const {
    return explosion_radius;
}

const UINT16Serializer &HelloMessage::get_bomb_timer() const {
    return bomb_timer;
}

std::string AcceptedPlayerMessage::serialize() {
    std::string result{static_cast<char>(message_id)};
    result += player_id.serialize();
    result += player.serialize();

    return result;
}

const player_id_t &AcceptedPlayerMessage::get_player_id() const {
    return player_id;
}

const Player &AcceptedPlayerMessage::get_player() const {
    return player;
}

std::string GameStartedMessage::serialize() {
    std::string result{static_cast<char>(message_id)};
    result += players.serialize();

    return result;
}

MapSerializer<Player> &GameStartedMessage::get_players() {
    return players;
}

std::string TurnMessage::serialize() {
    std::string result{static_cast<char>(message_id)};
    result += turn.serialize();

    for (const auto &el : events) {
        result += el->serialize();
    }

    return result;
}

UINT16Serializer &TurnMessage::get_turn() {
    return turn;
}

std::vector<std::shared_ptr<Event>> &TurnMessage::get_events() {
    return events;
}

std::string GameEndedMessage::serialize() {
    std::string result{static_cast<char>(message_id)};
    result += scores.serialize();

    return result;
}
