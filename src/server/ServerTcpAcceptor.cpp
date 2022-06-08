#include <iostream>
#include "ServerTcpAcceptor.h"

void ServerTcpAcceptor::operator()() {
    try {
        while (true) {
           std::shared_ptr<tcp::socket> socket = std::make_shared<tcp::socket>(io_context);
            acceptor.accept(*socket);
            player_sockets.push_back(socket);
        }
    }
    catch (std::exception &exception) {
        std::cerr << exception.what() << std::endl;
        callback_function();
    }
}
