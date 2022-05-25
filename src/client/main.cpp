#include "arguments.h"
#include "../utils/serializer.h"
#include "../utils/utils.h"
#include "../utils/messages/DrawMessage.h"
#include "GuiConnector.h"
#include "client_connections.h"
#include "GuiHandler.h"
#include "ServerHandler.h"

#define GUI_ADDRESS "gui-address"
#define HELP "help"
#define PLAYER_NAME "player-name"
#define PORT "port"
#define SERVER_ADDRESS "server-address"

int main(int argc, char *argv[]) {
    Arguments arguments = parse_arguments(argc, argv);

    boost::asio::io_context io_context;
    GuiConnector gui_connector{io_context, arguments.getGuiAddress(), arguments.getGuiPort(), arguments.getClientPort()};
    ServerConnector server_connector{io_context, arguments.getServerAddress(), arguments.getServerPort()};
    ClientGameState game_state{};
    GuiHandler gui_handler{server_connector, gui_connector, game_state, arguments.getPlayerName()};
    ServerHandler server_handler{server_connector, gui_connector, game_state};

    std::thread gui_handler_thread(gui_handler);
    std::thread server_handler_thread(server_handler);

    while (true) {}

    return 0;
}
