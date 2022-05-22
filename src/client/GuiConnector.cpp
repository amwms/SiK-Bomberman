#include "GuiConnector.h"

void GuiConnector::send_message(const std::string &buffer) {
    socket.send_to(boost::asio::buffer(buffer), receiver_endpoint);
}

std::string GuiConnector::receive_message() {
    boost::array<char, 16> recv_buffer{};
    udp::endpoint sender_endpoint;
    size_t len = socket.receive_from(boost::asio::buffer(recv_buffer), sender_endpoint);

    return std::string{recv_buffer.data(), len};
}