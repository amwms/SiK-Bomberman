#include <memory>
#include "InputMessage.h"

std::string InputMessage::serialize() const {
    return std::string{static_cast<char>(message_id)};
}

std::string MoveGui::serialize() const {
    std::string result{static_cast<char>(message_id)};
    result += direction.serialize();

    return result;
}

std::shared_ptr<ClientMessage> PlaceBombGui::to_client_message() {
    return std::make_shared<PlaceBombServer>();
}

std::shared_ptr<ClientMessage> PlaceBlockGui::to_client_message() {
    return std::make_shared<PlaceBlockServer>();
}

std::shared_ptr<ClientMessage> MoveGui::to_client_message() {
    return std::make_shared<MoveServer>(direction);
}