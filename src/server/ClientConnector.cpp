#include "ClientConnector.h"

std::string ClientConnector::receive_message(size_t bytes) {
    size_t len = boost::asio::read(socket, boost::asio::buffer(buffer, bytes), boost::asio::transfer_exactly(bytes));
    assert(bytes == len);
    return std::string{buffer.data(), len};
}

void ClientConnector::send_message(const std::string &message) {
    size_t len = boost::asio::write(socket, boost::asio::buffer(message));
    assert(len == message.size());
}
