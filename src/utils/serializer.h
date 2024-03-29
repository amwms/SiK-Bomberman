#ifndef SIK_BOMBERMAN_SERIALIZER_H
#define SIK_BOMBERMAN_SERIALIZER_H

#include <string>
#include <vector>
#include <map>
#include <endian.h>
#include <netinet/in.h>

class Serializer {
public:
    virtual std::string serialize() const = 0;
};

class StringSerializer : public Serializer {
    std::string string;

public:
    StringSerializer(std::string _string) : string(std::move(_string)) {}

    std::string serialize() const override;

    std::string get_string() const;
};

class UINT8Serializer : public Serializer {
    uint8_t num;

public:
    UINT8Serializer(uint8_t _num) : num(_num) {}

    std::string serialize() const override;

    [[maybe_unused]] uint8_t &get_num();

    bool operator< (const UINT8Serializer &a) const;
};

class UINT16Serializer : public Serializer {
    uint16_t num;

public:
    UINT16Serializer(uint16_t _num) : num(_num) {}

    std::string serialize() const override;

    uint16_t &get_num() {
        return num;
    }

    bool operator< (const UINT16Serializer &a) const;

    bool operator== (const UINT16Serializer &a) const;
};

class UINT32Serializer : public Serializer {
    uint32_t num;

public:
    UINT32Serializer(uint32_t _num) : num(_num) {}

    std::string serialize() const override;

    uint32_t &get_num();

    bool operator< (const UINT32Serializer &a) const;
};

template <class T>
requires std::derived_from<T, Serializer>
class ListSerializer : public Serializer {
    std::vector<T> list;

public:
    ListSerializer(const std::vector<T> &_list) : list(_list) {}

    std::string serialize() const override {
        char buffer[4];
        *((uint32_t *) buffer) = htonl((uint32_t) list.size());

        std::string result{buffer, 4};
        for (auto &el : list) {
            result += el.serialize();
        }

        return result;
    }

    const std::vector<T> &get_list() const {
        return list;
    }
};

template <class T>
requires std::derived_from<T, Serializer>
class MapSerializer : public Serializer {
    using key_t = UINT8Serializer;
    std::map<key_t, T> map;

public:
    MapSerializer(const std::map<key_t, T> _map) : map(_map) {}
    MapSerializer() : map() {}

    std::string serialize() const override {
        char buffer[4];
        *((uint32_t *) buffer) = htonl((uint32_t) map.size());

        std::string result{buffer, 4};
        for (auto &[key, value] : map) {
            result += key.serialize();
            result += value.serialize();
        }

        return result;
    }

    std::map<key_t, T> &get_map() {
        return map;
    }
};

#endif //SIK_BOMBERMAN_SERIALIZER_H
