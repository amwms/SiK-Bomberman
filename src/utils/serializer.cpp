#include <cassert>
#include "serializer.h"

std::string StringSerializer::serialize() {
    char buffer[1];
    assert(string.size() < 256);
    *buffer = static_cast<char>((uint8_t) string.size());

    return std::string{buffer, 1} + string;
}

std::string UINT16Serializer::serialize() {
    char buffer[2];
    *((uint16_t *) buffer) = htons(num);

    return buffer;
}
