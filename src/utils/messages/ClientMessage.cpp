#include "ClientMessage.h"

std::string ClientMessage::serialize() const {
    return std::string{static_cast<char>(message_id)};
}

uint8_t ClientMessage::get_message_id() {
    return message_id;
}

std::string JoinServer::serialize() const {
    std::string result{static_cast<char>(message_id)};
    result += name.serialize();

    return result;
}

std::string JoinServer::get_name() {
    return name.get_string();
}

std::string MoveServer::serialize() const {
    std::string result{static_cast<char>(message_id)};
    result += direction.serialize();

    return result;
}

const Direction &MoveServer::get_direction() const {
    return direction;
}
