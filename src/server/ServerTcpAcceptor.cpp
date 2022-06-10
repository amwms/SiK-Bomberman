#include <iostream>
#include "ServerTcpAcceptor.h"

void ServerTcpAcceptor::operator()() {
    try {
        while (true) {
            std::shared_ptr<tcp::socket> socket = std::make_shared<tcp::socket>(io_context);
            acceptor.accept(*socket);

            auto queue_sending = std::make_shared<ConcurrentQueue<std::string>>();
            auto queue_receiving = std::make_shared<
                    ConcurrentQueue<std::variant<JoinServer, PlaceBombServer, PlaceBlockServer, MoveServer>>>();
            auto pointer = std::make_shared<ClientHandler>(client_connector, game_state, callback_function,
                                                           queue_sending, queue_receiving);

            client_handlers.insert(pointer);

            // TODO - add Helo messages to send message queue
        }
    }
    catch (std::exception &exception) {
        std::cerr << exception.what() << std::endl;
        callback_function();
    }
}
