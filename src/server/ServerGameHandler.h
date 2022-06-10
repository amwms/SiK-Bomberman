#ifndef SIK_BOMBERMAN_SERVERGAMEHANDLER_H
#define SIK_BOMBERMAN_SERVERGAMEHANDLER_H

#include <functional>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "../utils/utils.h"
#include "ServerGameState.h"
#include "../utils/ConcurrentQueue.h"
#include "ClientHandler.h"

using boost::asio::ip::tcp;

class ServerGameHandler {
    using client_receiving_queue_t =
        std::queue<std::variant<JoinServer, PlaceBombServer, PlaceBlockServer, MoveServer>>;
    using action_t = std::variant<JoinServer, PlaceBombServer, PlaceBlockServer, MoveServer>;

    ServerGameState &game_state;
    std::function<void(void)> callback_function;
    std::set<std::shared_ptr<ClientHandler>> &client_handlers;

    void handle_game_turn();
    void handle_join_server(JoinServer &action, ClientHandler &client_handler);
    void handle_new_joins();
    void handle_game();
    void handle_lobby();

    void send_message(const std::basic_string<char> &message);

    void lock_all_receiving_queues_in_player_clients();
    void unlock_all_receiving_queues_in_player_clients();

    void clear_client_handlers_as_players();
    void clean_all_client_queues();
    action_t get_last_action_in_queue(client_receiving_queue_t &receiving_queue);

public:
    ServerGameHandler(ServerGameState &_game_state,
                      std::set<std::shared_ptr<ClientHandler>> &_client_handlers,
                      std::function<void(void)> &_callback_function) :
                        game_state(_game_state),
                        callback_function(_callback_function),
                        client_handlers(_client_handlers) {}

    void operator()();
};


#endif //SIK_BOMBERMAN_SERVERGAMEHANDLER_H
