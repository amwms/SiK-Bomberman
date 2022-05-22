#include "event.h"

std::string BombPlacedEvent::serialize() {
    std::string result;

    result += static_cast<char>(message_id);
    result += bomb_id.serialize();
    result += position.serialize();

    return result;
}

std::string BombExplodedEvent::serialize() {
    std::string result;

    result += static_cast<char>(message_id);
    result += bomb_id.serialize();
    result += robots_destroyed.serialize();
    result += blocks_destroyed.serialize();

    return result;
}

std::string PlayerMovedEvent::serialize() {
    std::string result;

    result += static_cast<char>(message_id);
    result += bomb_id.serialize();
    result += position.serialize();

    return result;
}

std::string BlockPlacedEvent::serialize() {
    std::string result;

    result += static_cast<char>(message_id);
    result += position.serialize();

    return result;
}
