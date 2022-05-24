#include "GuiHandler.h"
#include "../utils/deserialization.h"
#include "client_connections.h"

void GuiHandler::handle() {
    try {
        while (true) {
            std::string message_from_gui = gui_connector.receive_message();
            std::shared_ptr<InputMessage> input_message = string_to_input_message(message_from_gui);

            if (game_state.is_in_lobby()) {
                JoinServer message{name.get_string()};
                server_connector.send_message(message.serialize());
            } else {
                pass_from_gui_to_server(input_message, server_connector);
            }
        }
    }
    catch (std::exception &exception) {

    }
}

void GuiHandler::operator()() {
    handle();
}
