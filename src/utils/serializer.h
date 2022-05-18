#ifndef SIK_BOMBERMAN_SERIALIZER_H
#define SIK_BOMBERMAN_SERIALIZER_H

#include <string>
#include <vector>

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

template <typename T>
class ListSerializer : public Serializer {
    std::vector<T> list;
public:
    ListSerializer(const std::vector<T> list) : list(list) {}

    std::string serialize() override;

    const std::vector<T> &getList() const {
        return list;
    }
};

#endif //SIK_BOMBERMAN_SERIALIZER_H
