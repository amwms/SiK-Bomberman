#ifndef SIK_BOMBERMAN_RANDOMNUMBERGENERATOR_H
#define SIK_BOMBERMAN_RANDOMNUMBERGENERATOR_H

#include <cstdint>
#include <random>

class RandomNumberGenerator {
    uint32_t seed;
    std::minstd_rand random;

public:
    RandomNumberGenerator(uint32_t _seed) : seed(_seed), random(seed) {}

    uint32_t get_number();
};


#endif //SIK_BOMBERMAN_RANDOMNUMBERGENERATOR_H
