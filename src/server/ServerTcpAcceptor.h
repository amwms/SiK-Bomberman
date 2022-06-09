#ifndef SIK_BOMBERMAN_SERVERTCPACCEPTOR_H
#define SIK_BOMBERMAN_SERVERTCPACCEPTOR_H

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "../utils/utils.h"

using boost::asio::ip::tcp;

class ServerTcpAcceptor {
    UINT8Serializer counter = 0;
    using port_t = uint16_t;

    boost::asio::io_context &io_context;
    tcp::acceptor acceptor;
    std::map<player_id_t, std::shared_ptr<tcp::socket>> &player_sockets;
    std::function<void(void)> callback_function;

public:
    ServerTcpAcceptor(boost::asio::io_context &_io_context, port_t _port,
                      std::map<player_id_t, std::shared_ptr<tcp::socket>> &_player_sockets,
                      std::function<void(void)> &_callback_function) :
                        io_context(_io_context),
                        acceptor(io_context, tcp::endpoint(tcp::v6(), _port)),
                        player_sockets(_player_sockets),
                        callback_function(_callback_function) {}

    void operator()();
};

#endif //SIK_BOMBERMAN_SERVERTCPACCEPTOR_H
