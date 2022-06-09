#ifndef SIK_BOMBERMAN_SERVERGAMEHANDLER_H
#define SIK_BOMBERMAN_SERVERGAMEHANDLER_H

#include <functional>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "../utils/utils.h"
#include "ServerGameState.h"
#include "../utils/ConcurrentQueue.h"

using boost::asio::ip::tcp;

class ServerGameHandler {
    using queue_map_t = std::map<player_id_t, std::shared_ptr<ConcurrentQueue<std::string>>>;

    ServerGameState &game_state;
    std::function<void(void)> callback_function;
    std::map<player_id_t, std::shared_ptr<ConcurrentQueue<std::string>>> &player_send_message_queues;

    void handle_game_turn();

    void send_message(TurnMessage &message);

public:
    ServerGameHandler(ServerGameState &_game_state,
                      queue_map_t &_player_send_message_queues,
                      std::function<void(void)> &_callback_function) :
                        game_state(_game_state),
                        callback_function(_callback_function),
                        player_send_message_queues(_player_send_message_queues){}

    void operator()();
};


#endif //SIK_BOMBERMAN_SERVERGAMEHANDLER_H
