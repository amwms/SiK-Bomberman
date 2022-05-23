#include "deserialization.h"

StringSerializer string_to_string_serializer(std::string &string) {
    return {string};
}

std::shared_ptr<InputMessage> string_to_input_message(std::string &string) {
    size_t len = string.size();
    if (len < 1)
        throw DeserializationException{};

    switch (string[0]) {
        case 0:
            if (len > 1) throw DeserializationException{};
            return std::make_shared<PlaceBombGui>();
        case 1:
            if (len > 1) throw DeserializationException{};
            return std::make_shared<PlaceBlockGui>();
        case 2:
            if (len > 2) throw DeserializationException{};
            return std::make_shared<MoveGui>(string[1]);
        default:
            throw DeserializationException{};
    }
}
