#ifndef SIK_BOMBERMAN_SERIALIZER_H
#define SIK_BOMBERMAN_SERIALIZER_H

#include <string>
#include <vector>
#include <map>
#include <endian.h>
#include <netinet/in.h>

class Serializer {
public:
    virtual std::string serialize() = 0;
};

class StringSerializer : public Serializer {
    std::string string;

public:
    StringSerializer(std::string _string) : string(std::move(_string)) {}

    std::string serialize() override;

    const std::string &getString() const {
        return string;
    }
};

class UINT16Serializer : public Serializer {
    uint16_t num;

public:
    UINT16Serializer(uint16_t _num) : num(_num) {}

    std::string serialize() override;

    uint16_t getNum() const {
        return num;
    }
};

class UINT32Serializer : public Serializer {
    uint32_t num;

public:
    UINT32Serializer(uint32_t _num) : num(_num) {}

    std::string serialize() override;

    uint32_t getNum() const {
        return num;
    }
};

template <class T>
requires std::derived_from<T, Serializer>
class ListSerializer : public Serializer {
    std::vector<T> list;

public:
    ListSerializer(const std::vector<T> &_list) : list(_list) {}

    std::string serialize() override {
        char buffer[4];
        *((uint32_t *) buffer) = htonl((uint32_t) list.size());

        std::string result{buffer, 4};
        for (auto &el : list) {
            result += el.serialize();
        }

        return result;
    }

    const std::vector<T> &getList() const {
        return list;
    }
};

template <class T>
requires std::derived_from<T, Serializer>
class MapSerializer : public Serializer {
    using key_t = uint8_t;
    std::map<key_t, T> map;

public:
    MapSerializer(const std::map<key_t, T> _map) : map(_map) {}

    std::string serialize() override {
        char buffer[4];
        *((uint32_t *) buffer) = htonl((uint32_t) map.size());

        std::string result{buffer, 4};
        for (auto &el : map) {
            result += el.first; // this is type uint8_t so it is already serialized
            result += el.second.serialize();
        }

        return result;
    }

    const std::map<key_t, T> &getMap() const {
        return map;
    }
};

#endif //SIK_BOMBERMAN_SERIALIZER_H
