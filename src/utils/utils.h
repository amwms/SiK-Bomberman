#ifndef SIK_BOMBERMAN_UTILS_H
#define SIK_BOMBERMAN_UTILS_H

#include "serializer.h"

using score_t = UINT32Serializer;
using player_id_t = UINT8Serializer;
using bomb_id_t = UINT32Serializer;

class Player : public Serializer {
    StringSerializer name;
    StringSerializer address;

public:
    Player(const std::string &_name, const std::string &_address) : name(_name), address(_address) {}
    Player(StringSerializer &_name, StringSerializer &_address) : name(_name), address(_address) {}

    std::string serialize() const override;
};

class Position : public Serializer {
    UINT16Serializer x;
    UINT16Serializer y;

public:
    Position(uint16_t _x, uint16_t _y) : x(_x), y(_y) {}
    Position(UINT16Serializer &_x, UINT16Serializer &_y) : x(_x), y(_y) {}

    std::string serialize() const override;
};

class Bomb : public Serializer {
    Position position;
    UINT16Serializer timer;

public:
    Bomb(const Position &_position, uint16_t _timer) : position(_position), timer(_timer) {}

    std::string serialize() const override;
};

class Direction : public Serializer {
    uint8_t direction_id;

public:
    Direction(uint8_t _direction_id) : direction_id(_direction_id) {}

    std::string serialize() const override;
};

#endif //SIK_BOMBERMAN_UTILS_H
