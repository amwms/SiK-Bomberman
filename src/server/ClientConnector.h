#ifndef SIK_BOMBERMAN_CLIENTCONNECTOR_H
#define SIK_BOMBERMAN_CLIENTCONNECTOR_H

#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class ClientConnector {
    static const size_t buffer_size = 1e6;

    boost::asio::io_context &io_context;
    tcp::socket &socket;
    boost::array<char, buffer_size> buffer;

public:
    ClientConnector(boost::asio::io_context &_io_context,
                    tcp::socket &_socket) : io_context(_io_context), socket(_socket) {}

    void send_message(const std::string &buffer);

    std::string receive_message(size_t bytes);
};


#endif //SIK_BOMBERMAN_CLIENTCONNECTOR_H
