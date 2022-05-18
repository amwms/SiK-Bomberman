#include <cassert>
#include "serializer.h"

std::string StringSerializer::serialize() {
    char buffer[1];
    assert(string.size() < 256);
    *buffer = static_cast<char>((uint8_t) string.size());

    return std::string{buffer, 1} + string;
}

template<typename T>
std::string ListSerializer<T>::serialize() {
    char buffer[4];
    *((uint32_t *) buffer) = htonl(list.size());

    std::string result{buffer, 4};
    for (auto &el : list) {
        result += el.serlialize();
    }

    return result;
}


