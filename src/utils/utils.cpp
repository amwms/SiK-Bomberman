#include "utils.h"

std::string Player::serialize() const {
    std::string result;

    result += name.serialize();
    result += address.serialize();

    return result;
}

std::string Position::serialize() const {
    std::string result;

    result += x.serialize();
    result += y.serialize();

    return result;
}


std::string Bomb::serialize() const {
    std::string result;

    result += position.serialize();
    result += timer.serialize();

    return result;
}

std::string Direction::serialize() const {
    return std::string{static_cast<char>(direction_id)};
}
