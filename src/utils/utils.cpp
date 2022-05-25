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

const Position &Bomb::get_position() const {
    return position;
}

void Bomb::dec_timer() {
    timer.get_num()--;
}

std::string Direction::serialize() const {
    return std::string{static_cast<char>(direction_id)};
}
