#include <cassert>
#include "serializer.h"

std::string StringSerializer::serialize() const {
    char buffer[1];
    assert(string.size() < 256);
    *buffer = static_cast<char>((uint8_t) string.size());

    return std::string{buffer, 1} + string;
}

std::string StringSerializer::get_string() const  {
    return string;
}

std::string UINT16Serializer::serialize() const {
    char buffer[2];
    *((uint16_t *) buffer) = htons(num);

    return {buffer, 2};
}

bool UINT16Serializer::operator<(const UINT16Serializer &a) const {
    return this->num < a.num;
}

bool UINT16Serializer::operator==(const UINT16Serializer &a) const {
    return this->num == a.num;
}

std::string UINT32Serializer::serialize() const {
    char buffer[4];
    *((uint32_t *) buffer) = htonl(num);

    return {buffer, 4};
}

uint32_t &UINT32Serializer::get_num() {
    return num;
}

bool UINT32Serializer::operator<(const UINT32Serializer &a) const {
    return this->num < a.num;
}

std::string UINT8Serializer::serialize() const {
    char buffer[1];
    *((uint8_t *) buffer) = num;

    return {buffer, 1};
}

[[maybe_unused]] uint8_t &UINT8Serializer::get_num() {
    return num;
}

bool UINT8Serializer::operator<(const UINT8Serializer &a) const {
    return this->num < a.num;
}
