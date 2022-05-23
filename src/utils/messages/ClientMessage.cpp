#include "ClientMessage.h"

std::string ClientMessage::serialize() {
    return std::string{static_cast<char>(message_id)};
}

std::string JoinServer::serialize() {
    std::string result{static_cast<char>(message_id)};
    result += name.serialize();

    return result;
}

std::string MoveServer::serialize() {
    std::string result{static_cast<char>(message_id)};
    result += direction.serialize();

    return result;
}
