#ifndef SIK_BOMBERMAN_UTILS_H
#define SIK_BOMBERMAN_UTILS_H

#include "serializer.h"

using score_t = UINT32Serializer;
using playerid_t = uint8_t;
using bombid_t = UINT32Serializer;

class Player : public Serializer {
    StringSerializer name;
    StringSerializer address;

public:
    Player(const std::string &_name, const std::string &_address) : name(_name), address(_address) {}

    std::string serialize() override;
};

class Position : public Serializer {
    UINT16Serializer x;
    UINT16Serializer y;

public:
    Position(uint16_t _x, uint16_t _y) : x(_x), y(_y) {}

    std::string serialize() override;
};

class Bomb : public Serializer {
    Position position;
    UINT16Serializer timer;

public:
    Bomb(const Position &_position, uint16_t _timer) : position(_position), timer(_timer) {}

    std::string serialize() override;
};

#endif //SIK_BOMBERMAN_UTILS_H
