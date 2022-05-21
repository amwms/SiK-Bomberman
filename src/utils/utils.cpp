#include "utils.h"

std::string Player::serialize() {
    std::string result;

    result += name.serialize();
    result += address.serialize();

    return result;
}

std::string Position::serialize() {
    std::string result;

    result += x.serialize();
    result += y.serialize();

    return result;
}


std::string Bomb::serialize() {
    std::string result;

    result += position.serialize();
    result += timer.serialize();

    return result;
}
