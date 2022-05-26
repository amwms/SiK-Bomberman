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
using address_t = std::string;

#define GUI_ADDRESS "gui-address"
#define HELP "help"
#define PLAYER_NAME "player-name"
#define PORT "port"
#define SERVER_ADDRESS "server-address"

static void print_exit(auto &message) {
    std::cout << message << "\n";
    exit(1);
}

[[maybe_unused]] static void print_input_arguments(po::variables_map &map) {
    std::cout << GUI_ADDRESS" was set to: " << map[GUI_ADDRESS].as<std::string>() << "\n";
    std::cout << PLAYER_NAME" was set to: " << map[PLAYER_NAME].as<std::string>() << "\n";
    std::cout << PORT" was set to: " << map[PORT].as<port_t>() << "\n";
    std::cout << SERVER_ADDRESS" was set to: " << map[SERVER_ADDRESS].as<std::string>() << "\n";
}

static port_t get_port_from_address(const address_t &address) {
    size_t last_colon = address.find_last_of(':');
    return static_cast<port_t>(std::stoi(address.substr(last_colon + 1)));
}

static std::string get_address_from_address(const address_t &address) {
    size_t last_colon = address.find_last_of(':');
    return address.substr(0, last_colon);
}

static Arguments convert_parameters(po::variables_map &map) {
    ArgumentsBuilder args = ArgumentsBuilder();
    args = args.setPlayerName(map[PLAYER_NAME].as<std::string>())
            .setClientPort(map[PORT].as<port_t>())
            .setGuiPort(get_port_from_address(map[GUI_ADDRESS].as<std::string>()))
            .setServerPort(get_port_from_address(map[SERVER_ADDRESS].as<std::string>()))
            .setGuiAddress(get_address_from_address(map[GUI_ADDRESS].as<std::string>()))
            .setServerAddress(get_address_from_address(map[SERVER_ADDRESS].as<std::string>()));

    return args.build();
}

Arguments parse_arguments(int argc, char *argv[]) {
    // declare the supported options
    po::options_description desc("Program options");
    po::positional_options_description p;
    p.add("redundant", -1);

    desc.add_options()
            (HELP",h", "Wypisuje jak używać programu")
            (PLAYER_NAME",n", po::value<std::string>()->required(), "Nazwa gracza")
            (GUI_ADDRESS",d", po::value<std::string>()->required(),
             "<(nazwa hosta):(port) lub (IPv4):(port) lub (IPv6):(port)>")
            (PORT",p", po::value<uint16_t>()->required(), "Port na którym klient nasłuchuje komunikatów od GUI")
            (SERVER_ADDRESS",s", po::value<std::string>()->required(),
             "<(nazwa hosta):(port) lub (IPv4):(port) lub (IPv6):(port)>");

    po::variables_map variables_map;

    try {
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), variables_map);

        // if the help argument was given then show help options of the program
        if (variables_map.count("help")) {
            print_exit(desc);
            __builtin_unreachable();
        }

        po::notify(variables_map);

        if (variables_map.count("redundant")) {
            print_exit("Wrong arguments - call -h (--help) for correct program options.\n");
            __builtin_unreachable();
        }
    }
    catch (const po::error &exception) {
        std::cerr << exception.what() << std::endl;
        exit(1);
    }

    return convert_parameters(variables_map);
}

Arguments ArgumentsBuilder::build() {
    return {gui_port, player_name, client_port, server_port, server_address, gui_address};
}

ArgumentsBuilder ArgumentsBuilder::setGuiPort(port_t guiPort) {
    gui_port = guiPort;
    return *this;
}

ArgumentsBuilder ArgumentsBuilder::setPlayerName(const std::string &playerName) {
    player_name = playerName;
    return *this;
}

ArgumentsBuilder ArgumentsBuilder::setClientPort(port_t clientPort) {
    client_port = clientPort;
    return *this;
}

ArgumentsBuilder ArgumentsBuilder::setServerPort(port_t serverPort) {
    server_port = serverPort;
    return *this;
}

ArgumentsBuilder ArgumentsBuilder::setServerAddress(const std::string &serverAddress) {
    server_address = serverAddress;
    return *this;
}

ArgumentsBuilder ArgumentsBuilder::setGuiAddress(const std::string &guiAddress) {
    gui_address = guiAddress;
    return *this;
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

const std::string &Arguments::getServerAddress() const {
    return server_address;
}

const std::string &Arguments::getGuiAddress() const {
    return gui_address;
}

