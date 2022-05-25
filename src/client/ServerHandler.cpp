#include "ServerHandler.h"
#include "../utils/messages/ServerMessage.h"
#include "../utils/deserialization.h"
#include "../utils/messages/DrawMessage.h"

#define HELLO 0
#define ACCEPTED_PLAYER 1
#define GAME_STARTED 2
#define TURN 3
#define GAME_ENDED 4

void handle_hello_message(ServerConnector &server_connector, ClientGameState &game_state) {
    HelloMessage hello_message =  tcp_deserialize_to_hello_message(server_connector);

    game_state.server_name = hello_message.get_server_name();
    game_state.players_count = hello_message.get_players_count();
    game_state.size_x = hello_message.get_size_x();
    game_state.size_y = hello_message.get_size_y();
    game_state.game_length = hello_message.get_game_length();
    game_state.explosion_radius = hello_message.get_explosion_radius();
    game_state.bomb_timer = hello_message.get_bomb_timer();
}

void handle_accepted_player_message(ServerConnector &server_connector, ClientGameState &game_state) {
    AcceptedPlayerMessage accepted_player_message = tcp_deserialize_to_accepted_player_message(server_connector);

    game_state.players
              .get_map()
              .insert({accepted_player_message.get_player_id(), accepted_player_message.get_player()});
}

void handle_game_started_message(ServerConnector &server_connector, ClientGameState &game_state) {
    GameStartedMessage game_started_message = tcp_deserialize_to_game_started_message(server_connector);

    game_state.players = game_started_message.get_players();
    game_state.in_lobby = false;
}

void handle_turn_message(ServerConnector &server_connector, ClientGameState &game_state) {
    TurnMessage turn_message = tcp_deserialize_to_turn_message(server_connector);

    game_state.turn = turn_message.get_turn();

    auto events = turn_message.get_events();
    for (auto &event : events) {
        event->handle(game_state);
    }
}

void handle_game_ended_message(ServerConnector &server_connector, ClientGameState &game_state) {
    GameEndedMessage game_ended_message = tcp_deserialize_to_game_ended_message(server_connector);

    game_state.scores = game_ended_message.getScores();
    game_state.in_lobby = true;
}

void ServerHandler::handle() {
    std::string message_id = server_connector.receive_message(1);
    assert(message_id.size() == 1);

    switch (message_id[0]) {
        case HELLO:
            handle_hello_message(server_connector, game_state);
            break;
        case ACCEPTED_PLAYER:
            handle_accepted_player_message(server_connector, game_state);
            break;
        case GAME_STARTED:
            handle_game_started_message(server_connector, game_state);
            break;
        case TURN:
            handle_turn_message(server_connector, game_state);
            break;
        case GAME_ENDED:
            handle_game_ended_message(server_connector, game_state);
            break;
        default:
            throw DeserializationException{};
    }
}

void ServerHandler::operator()() {
    try {
        while (true) {
            handle();
        }
    }
    catch (std::exception &exception) {

    }
}
