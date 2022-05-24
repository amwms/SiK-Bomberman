#include "ClientMessage.h"

std::string ClientMessage::serialize() const {
    return std::string{static_cast<char>(message_id)};
}

std::string JoinServer::serialize() const {
    std::string result{static_cast<char>(message_id)};
    result += name.serialize();

    return result;
}

std::string MoveServer::serialize() const {
    std::string result{static_cast<char>(message_id)};
    result += direction.serialize();

    return result;
}
