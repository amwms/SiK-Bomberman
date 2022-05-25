#ifndef SIK_BOMBERMAN_GUICONNECTOR_H
#define SIK_BOMBERMAN_GUICONNECTOR_H

#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class GuiConnector {
    using port_t = uint16_t;

    boost::asio::io_context &io_context;
    udp::resolver resolver;
    udp::endpoint receiver_endpoint;
    udp::socket socket;

public:
    GuiConnector(boost::asio::io_context &_io_context, const std::string &_address, port_t _gui_port, port_t _port) :
        io_context(_io_context),
        resolver(_io_context),
        receiver_endpoint(*resolver.resolve(_address, std::to_string(_gui_port))),
        socket(_io_context, udp::endpoint{udp::v6(), _port}) {}

    void send_message(const std::string &buffer);

    std::string receive_message();
};


#endif //SIK_BOMBERMAN_GUICONNECTOR_H
