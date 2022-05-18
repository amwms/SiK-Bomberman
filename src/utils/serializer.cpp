#include <cassert>
#include "serializer.h"

std::string StringSerializer::serialize() {
    char buffer[1];
    assert(string.size() < 256);
    *buffer = static_cast<char>((uint8_t) string.size());

    return std::string{buffer, 1} + string;
}

template <class T, class U>
requires std::derived_from<T, Serializer> && std::derived_from<U, Serializer>
std::string MapSerializer<T, U>::serialize() {
    char buffer[4];
    *((uint32_t *) buffer) = htonl(map.size());

    std::string result{buffer, 4};
    for (auto &el : map) {
        result += el.first.serlialize();
        result += el.second.serlialize();
    }

    return result;
}

