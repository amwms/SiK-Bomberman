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

template <class T>
requires std::derived_from<T, Serializer>
class ListSerializer : public Serializer {
    std::vector<T> list;
public:
    ListSerializer(const std::vector<T> list) : list(list) {}

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

template <class T, class U>
requires std::derived_from<T, Serializer> && std::derived_from<U, Serializer>
class MapSerializer : public Serializer {
    std::map<T, U> map;
public:
    MapSerializer(const std::map<T, U> _map) : map(_map) {}

    std::string serialize() override;

    const std::map<T, U> &getMap() const {
        return map;
    }
};

#endif //SIK_BOMBERMAN_SERIALIZER_H
