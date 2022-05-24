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

std::string AcceptedPlayerMessage::serialize() {
    std::string result{static_cast<char>(message_id)};
    result += player_id.serialize();
    result += player.serialize();

    return result;
}

std::string GameStartedMessage::serialize() {
    std::string result{static_cast<char>(message_id)};
    result += players.serialize();

    return result;
}

std::string TurnMessage::serialize() {
    std::string result{static_cast<char>(message_id)};
    result += turn.serialize();

    for (const auto &el : events) {
        result += el->serialize();
    }

    return result;
}

std::string GameEndedMessage::serialize() {
    std::string result{static_cast<char>(message_id)};
    result += scores.serialize();

    return result;
}
