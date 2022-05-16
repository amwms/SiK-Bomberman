#include "arguments.h"
#include <boost/program_options.hpp>
#include <string>
#include <iostream>

namespace po = boost::program_options;

//-d, --gui-address <(nazwa hosta):(port) lub (IPv4):(port) lub (IPv6):(port)>
//-h, --help                                 Wypisuje jak używać programu
//-n, --player-name <String>
//-p, --port <u16>                           Port na którym klient nasłuchuje komunikatów od GUI
//-s, --server-address <(nazwa hosta):(port) lub (IPv4):(port) lub (IPv6):(port)>

using port_t = uint16_t;

#define GUI_ADDRESS "gui-address"
#define HELP "help"
#define PLAYER_NAME "player-name"
#define PORT "port"
#define SERVER_ADDRESS "server-address"


void print_exit(auto &message) {
    std::cout << message << "\n";
    exit(1);
}

void print_input_arguments(po::variables_map &map) {
    std::cout << GUI_ADDRESS" was set to: " << map[GUI_ADDRESS].as<std::string>() << "\n";
    std::cout << PLAYER_NAME" was set to: " << map[PLAYER_NAME].as<std::string>() << "\n";
    std::cout << PORT" was set to: " << map[PORT].as<port_t>() << "\n";
    std::cout << SERVER_ADDRESS" was set to: " << map[SERVER_ADDRESS].as<std::string>() << "\n";
}

Arguments parse_arguments(int argc, char *argv[]) {
    // Declare the supported options.
    po::options_description desc("Program options:");
    desc.add_options()
            (HELP" ,h", "Wypisuje jak używać programu")
            (PLAYER_NAME" ,n", po::value<std::string>()->required(), "Nazwa gracza")
            (GUI_ADDRESS" ,d", po::value<std::string>()->required(),
             "<(nazwa hosta):(port) lub (IPv4):(port) lub (IPv6):(port)>")
            (PORT" ,p", po::value<uint16_t>()->required(), "Port na którym klient nasłuchuje komunikatów od GUI")
            (SERVER_ADDRESS" ,s", po::value<std::string>(),
             "<(nazwa hosta):(port) lub (IPv4):(port) lub (IPv6):(port)>");

    po::variables_map variables_map;

    try {
        po::store(po::parse_command_line(argc, argv, desc), variables_map);
        po::notify(variables_map);
    }
    catch (const po::error &except) {
        std::cerr << except.what() << '\n';
        exit(1);
    }

    // if the help argument was given then show help options of the program
    if (variables_map.count("help")) {
        print_exit(desc);
        __builtin_unreachable();
    }

    print_input_arguments(variables_map);

    return Arguments();
}

Arguments ArgumentsBuilder::build() {
    return Arguments(gui_port, player_name, client_port, server_port);
}

void ArgumentsBuilder::setGuiPort(port_t guiPort) {
    gui_port = guiPort;
}

void ArgumentsBuilder::setPlayerName(const std::string &playerName) {
    player_name = playerName;
}

void ArgumentsBuilder::setClientPort(port_t clientPort) {
    client_port = clientPort;
}

void ArgumentsBuilder::setServerPort(port_t serverPort) {
    server_port = serverPort;
}

const std::string &Arguments::getPlayerName() const {
    return player_name;
}

port_t Arguments::getGuiPort() const {
    return gui_port;
}

port_t Arguments::getClientPort() const {
    return client_port;
}

port_t Arguments::getServerPort() const {
    return server_port;
}

