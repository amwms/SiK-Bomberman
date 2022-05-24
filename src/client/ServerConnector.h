#ifndef SIK_BOMBERMAN_SERVERCONNECTOR_H
#define SIK_BOMBERMAN_SERVERCONNECTOR_H

#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class ServerConnector {
    using port_t = uint16_t;
    static const size_t buffer_size = 1e6;

    boost::asio::io_context &io_context;
    tcp::socket socket;
    boost::array<char, buffer_size> buffer;

public:
    ServerConnector(boost::asio::io_context &_io_context, const std::string &_address,
                    port_t _server_port) : io_context(_io_context), socket(_io_context) {
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve(_address, std::to_string(_server_port));
        boost::asio::connect(socket, endpoints);
    }

    void send_message(const std::string &buffer);

    std::string receive_message(size_t bytes);
};

#endif //SIK_BOMBERMAN_SERVERCONNECTOR_H
