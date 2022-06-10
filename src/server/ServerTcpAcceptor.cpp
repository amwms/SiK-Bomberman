#include <iostream>
#include "ServerTcpAcceptor.h"

static void send_message(const std::basic_string<char> &message, const std::shared_ptr<ClientHandler> &client_handler) {
    client_handler->client_sending_queue->push(message);
}

static void handle_welcome_message(ServerGameState &game_state, const std::shared_ptr<ClientHandler> &client_handler) {
    HelloMessage hello_message = game_state.to_hello_message();
    send_message(hello_message.serialize(), client_handler);

    if (game_state.is_lobby) {
        for (auto &player_it : game_state.players.get_map()) {
            player_id_t player_id = player_it.first;
            Player player = player_it.second;

            AcceptedPlayerMessage accepted_player_message{player_id, player};
            send_message(accepted_player_message.serialize(), client_handler);
        }
    }
    else {
        GameStartedMessage game_started_message{game_state.players};
        send_message(game_started_message.serialize(), client_handler);
    }
}


void ServerTcpAcceptor::operator()() {
    try {
        while (true) {
            std::shared_ptr<tcp::socket> socket = std::make_shared<tcp::socket>(io_context);
            acceptor.accept(*socket);

            auto queue_sending = std::make_shared<ConcurrentQueue<std::string>>();
            auto queue_receiving = std::make_shared<
                    ConcurrentQueue<std::variant<JoinServer, PlaceBombServer, PlaceBlockServer, MoveServer>>>();
            std::shared_ptr<ClientConnector> client_connector = std::make_shared<ClientConnector>(socket);

            auto client_pointer = std::make_shared<ClientHandler>(client_connector, game_state, callback_function,
                                                           queue_sending, queue_receiving);

            client_handlers.insert(client_pointer);
            handle_welcome_message(game_state, client_pointer);
        }
    }
    catch (std::exception &exception) {
        std::cerr << exception.what() << std::endl;
        callback_function();
    }
}
