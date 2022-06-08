#ifndef SIK_BOMBERMAN_RANDOMNUMBERGENERATOR_H
#define SIK_BOMBERMAN_RANDOMNUMBERGENERATOR_H

#include <cstdint>

class RandomNumberGenerator {
    uint32_t seed;
    uint32_t last_number = 0;
    bool generated_r0 = false;

public:
    RandomNumberGenerator(uint32_t seed) : seed(seed) {}

    uint32_t get_number();
};


#endif //SIK_BOMBERMAN_RANDOMNUMBERGENERATOR_H
