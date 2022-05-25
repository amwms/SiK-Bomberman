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

void Position::inc_x() {
    x.get_num()++;
}

void Position::inc_y() {
    y.get_num()++;
}

void Position::dec_x() {
    x.get_num()--;
}

void Position::dec_y() {
    y.get_num()--;
}

uint16_t Position::get_x() {
    return x.get_num();
}

uint16_t Position::get_y() {
    return y.get_num();
}

std::string Bomb::serialize() const {
    std::string result;

    result += position.serialize();
    result += timer.serialize();

    return result;
}

const Position &Bomb::getPosition() const {
    return position;
}

const UINT16Serializer &Bomb::getTimer() const {
    return timer;
}

void Bomb::setTimer(const UINT16Serializer &_timer) {
    Bomb::timer = _timer;
}

std::string Direction::serialize() const {
    return std::string{static_cast<char>(direction_id)};
}
