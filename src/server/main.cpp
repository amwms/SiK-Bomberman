#include <iostream>
#include <vector>
#include <memory>
#include "ServerArguments.h"
#include "../utils/utils.h"
#include "ClientHandler.h"
#include "ServerTcpAcceptor.h"
#include "ServerGameHandler.h"
#include <boost/asio.hpp>
#include <set>

int main(int argc, char *argv[]) {
    try {
        ServerArguments arguments = parse_server_arguments(argc, argv);

        boost::asio::io_context io_context;
        RandomNumberGenerator randomizer{arguments.seed};
        std::set<std::shared_ptr<ClientHandler>> client_handlers;

        ServerGameState game_state{randomizer, arguments.server_name, arguments.players_count, arguments.size_x,
                                   arguments.size_y, arguments.game_length, arguments.explosion_radius,
                                   arguments.bomb_timer, arguments.initial_blocks, arguments.turn_duration};
        ServerTcpAcceptor tcp_acceptor{io_context, arguments.port, client_handlers, game_state};
        ServerGameHandler game_handler{game_state, client_handlers};

        std::thread tcp_acceptor_thread(std::ref(tcp_acceptor));
        std::thread game_handler_thread(std::ref(game_handler));

        while (true){}
    }
    catch (std::exception &exception) {}

    exit(1);
}