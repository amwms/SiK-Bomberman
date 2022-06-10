#include "RandomNumberGenerator.h"

uint32_t RandomNumberGenerator::get_number() {
    return static_cast<uint32_t>(random());
}
