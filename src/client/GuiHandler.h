#ifndef SIK_BOMBERMAN_GUIHANDLER_H
#define SIK_BOMBERMAN_GUIHANDLER_H

#include <memory>
#include "../utils/messages/InputMessage.h"
#include "GuiConnector.h"
#include "ServerConnector.h"

class GuiHandler {
    ServerConnector &server_connector;
    GuiConnector &gui_connector;

    [[noreturn]] void handle();

public:
    GuiHandler(ServerConnector &serverConnector,
               GuiConnector &guiConnector) : server_connector(serverConnector), gui_connector(guiConnector) {}

    void operator()();
};


#endif //SIK_BOMBERMAN_GUIHANDLER_H
