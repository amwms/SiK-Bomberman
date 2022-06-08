#include "RandomNumberGenerator.h"

uint32_t RandomNumberGenerator::get_number() {
    uint32_t result;
    if (generated_r0) {
        result = (last_number * 48271) % 2147483647;
    }
    else {
        result = (seed * 48271) % 2147483647;
        generated_r0 = true;
    }

    last_number = result;
    return result;
}
