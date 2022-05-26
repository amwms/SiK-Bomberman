#ifndef SIK_BOMBERMAN_GUIHANDLER_H
#define SIK_BOMBERMAN_GUIHANDLER_H

#include <memory>
#include "../utils/messages/InputMessage.h"
#include "GuiConnector.h"
#include "ServerConnector.h"
#include "ClientGameState.h"

class GuiHandler {
    ClientGameState &game_state;
    ServerConnector &server_connector;
    GuiConnector &gui_connector;
    StringSerializer name;
    std::function<void(void)> callback_function;

    void handle();

public:
    GuiHandler(ServerConnector &_server_connector,
               GuiConnector &_gui_connector,
               ClientGameState &_game_state,
               const std::string &_name,
               std::function<void(void)> &_callback_function) : game_state(_game_state),
                                                                server_connector(_server_connector),
                                                                gui_connector(_gui_connector), name(_name),
                                                                callback_function(_callback_function) {}

    void operator()();
};


#endif //SIK_BOMBERMAN_GUIHANDLER_H
