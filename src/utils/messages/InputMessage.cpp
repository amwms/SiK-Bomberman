#include "InputMessage.h"

std::string InputMessage::serialize() {
    return std::string{static_cast<char>(message_id)};
}

std::string MoveGui::serialize() {
    std::string result{static_cast<char>(message_id)};
    result += direction.serialize();

    return result;
}
