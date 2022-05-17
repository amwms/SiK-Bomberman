#include <iostream>
#include "arguments.h"

#define GUI_ADDRESS "gui-address"
#define HELP "help"
#define PLAYER_NAME "player-name"
#define PORT "port"
#define SERVER_ADDRESS "server-address"

void check_arguments_class(Arguments &arguments) {
    std::cout << "---------- Checking Arguments class ----------\n";
    std::cout << GUI_ADDRESS" was set to: " << arguments.getGuiAddress() << ", Port: " << arguments.getGuiPort() << "\n";
    std::cout << PLAYER_NAME" was set to: " << arguments.getPlayerName() << "\n";
    std::cout << PORT" was set to: " << arguments.getClientPort() << "\n";
    std::cout << SERVER_ADDRESS" was set to: " <<  arguments.getServerAddress() << ", Port: " << arguments.getServerPort() << "\n";
}

int main(int argc, char *argv[]) {
    Arguments arguments = parse_arguments(argc, argv);

    check_arguments_class(arguments);

    std::cout << "Hello, World!\n";
    return 0;
}
