#ifndef SIK_BOMBERMAN_CLIENTCONNECTOR_H
#define SIK_BOMBERMAN_CLIENTCONNECTOR_H

#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class ClientConnector {
    static const size_t buffer_size = 1e6;

    std::shared_ptr<tcp::socket> socket;
    boost::array<char, buffer_size> buffer;

public:
    ClientConnector(const std::shared_ptr<tcp::socket> &_socket) : socket(_socket) {}

    void send_message(const std::string &buffer);

    std::string receive_message(size_t bytes);

    std::string get_socket_address();
};


#endif //SIK_BOMBERMAN_CLIENTCONNECTOR_H
