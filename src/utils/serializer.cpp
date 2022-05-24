#include <cassert>
#include "serializer.h"

std::string StringSerializer::serialize() const {
    char buffer[1];
    assert(string.size() < 256);
    *buffer = static_cast<char>((uint8_t) string.size());

    return std::string{buffer, 1} + string;
}

std::string UINT16Serializer::serialize() const {
    char buffer[2];
    *((uint16_t *) buffer) = htons(num);

    return buffer;
}

std::string UINT32Serializer::serialize() const {
    char buffer[4];
    *((uint32_t *) buffer) = htonl(num);

    return buffer;
}

std::string UINT8Serializer::serialize() const {
    return std::string{static_cast<char>(num)};
}
