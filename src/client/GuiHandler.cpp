#include "GuiHandler.h"
#include "../utils/deserialization.h"
#include "client_connections.h"

[[noreturn]] void GuiHandler::handle() {
    while (true) {
        std::string message = gui_connector.receive_message();
        std::shared_ptr<InputMessage> input_message = string_to_input_message(message);
        pass_from_gui_to_server(input_message, server_connector);
    }
}

void GuiHandler::operator()() {
    handle();
}
