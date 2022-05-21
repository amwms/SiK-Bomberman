#ifndef SIK_BOMBERMAN_PLAYER_H
#define SIK_BOMBERMAN_PLAYER_H

#include "serializer.h"

class Player : public Serializer {
    StringSerializer name;
    StringSerializer address;
public:
    Player(const std::string &name, const std::string &address) : name({name}), address({address}) {}

    std::string serialize() override {
        std::string result;

        result += name.serialize();
        result += address.serialize();

        return result;
    }
};

#endif //SIK_BOMBERMAN_PLAYER_H
