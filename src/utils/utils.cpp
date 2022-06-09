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

bool Position::operator<(const Position &p) const  {
    if (this->x == p.x)
        return this->y < p.y;

    return this->x < p.x;
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

uint16_t Bomb::get_timer() {
    return timer.get_num();
}

std::string Direction::serialize() const {
    return std::string{static_cast<char>(direction_id)};
}

uint8_t Direction::get_direction_id() {
    return direction_id;
}
