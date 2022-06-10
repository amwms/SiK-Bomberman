#ifndef SIK_BOMBERMAN_SERVERTCPACCEPTOR_H
#define SIK_BOMBERMAN_SERVERTCPACCEPTOR_H

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "../utils/utils.h"
#include "ClientHandler.h"

using boost::asio::ip::tcp;

class ServerTcpAcceptor {
    using port_t = uint16_t;

    boost::asio::io_context &io_context;
    tcp::acceptor acceptor;
    std::set<std::shared_ptr<ClientHandler>> &client_handlers;
    std::function<void(void)> callback_function;

    ServerGameState &game_state;

public:
    ServerTcpAcceptor(boost::asio::io_context &_io_context, port_t _port,
                      std::set<std::shared_ptr<ClientHandler>> &_client_handlers,
                      std::function<void(void)> &_callback_function,
                      ServerGameState &_game_state) :
                        io_context(_io_context),
                        acceptor(io_context, tcp::endpoint(tcp::v6(), _port)),
                        client_handlers(_client_handlers),
                        callback_function(_callback_function),
                        game_state(_game_state) {}

    void operator()();
};

#endif //SIK_BOMBERMAN_SERVERTCPACCEPTOR_H
