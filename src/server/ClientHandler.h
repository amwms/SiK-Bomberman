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
    ClientConnector &client_connector;
    std::function<void(void)> callback_function;

    std::optional<player_id_t> player_id;

    std::thread client_receive;
    std::thread client_send;

    void handle_sending();
    void handle_receiving();

public:
    client_sending_queue_t client_sending_queue;
    client_receiving_queue_t client_receiving_queue;

    ClientHandler(ClientConnector &_client_connector, ServerGameState &_game_state,
                  std::function<void(void)> &_callback_function,
                  client_sending_queue_t _client_sending_queue,
                  client_receiving_queue_t _client_receiving_queue) :
                    game_state(_game_state),
                    client_connector(_client_connector),
                    callback_function(_callback_function),
                    player_id(),
                    client_receive([&]{ handle_receiving(); }),
                    client_send([&]{ handle_sending(); }),
                    client_sending_queue(std::move(_client_sending_queue)),
                    client_receiving_queue(std::move(_client_receiving_queue)) {}

    bool is_player();

    player_id_t get_player_id();
};

#endif //SIK_BOMBERMAN_CLIENTHANDLER_H
