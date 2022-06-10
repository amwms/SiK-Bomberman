#include "ClientHandler.h"

void ClientHandler::handle_sending() {

}

void ClientHandler::handle_receiving() {

}

bool ClientHandler::is_player() {
    return player_id.has_value();
}

player_id_t ClientHandler::get_player_id() {
    return player_id.value();
}

