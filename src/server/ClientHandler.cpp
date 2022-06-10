#include <iostream>
#include "ClientHandler.h"
#include "../utils/deserialization.h"

void ClientHandler::handle_sending() {
    try {
        while (true) {
            while (!client_sending_queue->is_empty()) {
                auto message = client_sending_queue->pop();
                client_connector->send_message(message);
            }

            sleep(1);
        }
    }
    catch (std::exception &exception) {
        std::cerr << exception.what() << std::endl;
        should_be_destroyed = true;
    }
}

void ClientHandler::handle_receiving() {
    try {
        while (true) {
            std::string message_id = client_connector->receive_message(1);
            assert(message_id.size() == 1);

            switch (message_id[0]) {
                case 0:
                    client_receiving_queue->push(tcp_deserialize_to_join_server_message(*client_connector));
                    break;
                case 1:
                    client_receiving_queue->push(tcp_deserialize_to_place_bomb_message());
                    break;
                case 2:
                    client_receiving_queue->push(tcp_deserialize_to_place_block_message());
                    break;
                case 3:
                    client_receiving_queue->push(tcp_deserialize_to_move_server_message(*client_connector));
                    break;
                default:
                    throw DeserializationException{};
            }
        }
    }
    catch (std::exception &exception) {
        std::cerr << exception.what() << std::endl;
        should_be_destroyed = true;
    }
}

bool ClientHandler::is_player() {
    return player_id.has_value();
}

player_id_t ClientHandler::get_player_id() {
    return player_id.value();
}

void ClientHandler::set_player_id(std::optional<player_id_t> id) {
    player_id = id;
}

