#include "DrawMessage.h"

std::string LobbyMessage::serialize() {
    std::string result;

    result += static_cast<char>(message_id);
    result += server_name.serialize();
    result += static_cast<char>(players_count);
    result += size_x.serialize();
    result += size_y.serialize();
    result += game_length.serialize();
    result += explosion_radius.serialize();
    result += bomb_timer.serialize();
    result += players.serialize();

    return result;
}

std::string GameMessage::serialize() {
    std::string result;

    result += static_cast<char>(message_id);
    result += server_name.serialize();
    result += size_x.serialize();
    result += size_y.serialize();
    result += game_length.serialize();
    result += turn.serialize();
    result += players.serialize();
    result += player_positions.serialize();
    result += blocks.serialize();
    result += bombs.serialize();
    result += explosions.serialize();
    result += scores.serialize();

    return result;
}
