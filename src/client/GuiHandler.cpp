#include <iostream>
#include "GuiHandler.h"
#include "../utils/deserialization.h"

size_t pass_from_gui_to_server(std::shared_ptr<InputMessage> &input_message, ServerConnector &connector) {
    std::shared_ptr<ClientMessage> client_message = input_message->to_client_message();
    connector.send_message(client_message->serialize());

    return client_message->serialize().size();
}

void GuiHandler::handle() {
    std::string message_from_gui = gui_connector.receive_message();
    std::shared_ptr<InputMessage> input_message = string_to_input_message(message_from_gui);

    if (game_state.in_lobby) {
        JoinServer message{name.get_string()};
        server_connector.send_message(message.serialize());
    }
    else {
        pass_from_gui_to_server(input_message, server_connector);
    }
}

void GuiHandler::operator()() {
    try {
        while (true) {
            try {
                handle();
            }
            catch (DeserializationException &wrong_gui_message_exception) {
                continue;
            }
        }
    }
    catch (std::exception &exception) {
        std::cerr << exception.what() << std::endl;
        callback_function();
    }
}
