#ifndef SIK_BOMBERMAN_SERVERHANDLER_H
#define SIK_BOMBERMAN_SERVERHANDLER_H

#include "ServerConnector.h"
#include "GuiConnector.h"
#include "ClientGameState.h"

class ServerHandler {
    ClientGameState &game_state;
    ServerConnector &server_connector;
    GuiConnector &gui_connector;

    void handle();
    void send_message_to_gui();

public:
    ServerHandler(ServerConnector &_server_connector, GuiConnector &_gui_connector,
                  ClientGameState &_client_game_state) :
                  game_state(_client_game_state),
                  server_connector(_server_connector),
                  gui_connector(_gui_connector) {}

    void operator()();
};


#endif //SIK_BOMBERMAN_SERVERHANDLER_H
