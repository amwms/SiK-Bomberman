#ifndef SIK_BOMBERMAN_CLIENTHANDLER_H
#define SIK_BOMBERMAN_CLIENTHANDLER_H

#include <optional>
#include <utility>
#include "ServerGameState.h"
#include "ClientConnector.h"
#include "../utils/ConcurrentQueue.h"

class ClientHandler {
    using client_receiving_queue_t =
            std::shared_ptr<ConcurrentQueue<std::variant<JoinServer, PlaceBombServer, PlaceBlockServer, MoveServer>>>;
    using client_sending_queue_t = std::shared_ptr<ConcurrentQueue<std::string>>;

    ServerGameState &game_state;
    std::optional<player_id_t> player_id;

    void handle_sending();
    void handle_receiving();

public:
    bool should_be_destroyed = false;

    std::shared_ptr<ClientConnector> client_connector;
    client_sending_queue_t client_sending_queue;
    client_receiving_queue_t client_receiving_queue;

private:
    std::thread client_receive;
    std::thread client_send;

public:
    ClientHandler(const std::shared_ptr<ClientConnector> &_client_connector, ServerGameState &_game_state,
                  client_sending_queue_t _client_sending_queue,
                  client_receiving_queue_t _client_receiving_queue) :
                    game_state(_game_state),
                    player_id(),
                    client_connector(_client_connector),
                    client_sending_queue(std::move(_client_sending_queue)),
                    client_receiving_queue(std::move(_client_receiving_queue)),
                    client_receive(&ClientHandler::handle_receiving, this),
                    client_send(&ClientHandler::handle_sending, this) {}

    ~ClientHandler() {
        client_connector->socket->close();
        client_send.join();
        client_receive.join();
    }

    bool is_player();

    player_id_t get_player_id();

    void set_player_id(std::optional<player_id_t> id);
};

#endif //SIK_BOMBERMAN_CLIENTHANDLER_H
