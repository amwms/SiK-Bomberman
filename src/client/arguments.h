#ifndef SIK_BOMBERMAN_ARGUMENTS_H
#define SIK_BOMBERMAN_ARGUMENTS_H

#include <cinttypes>
#include <string>
#include <utility>

class Arguments {
private:
    using port_t = uint16_t;

    port_t gui_port;
    std::string player_name;
    port_t client_port;
    port_t server_port;
    std::string server_address;
    std::string gui_address;

public:
    Arguments(port_t _gui_port, std::string _player_name, port_t _client_port, port_t _server_port,
              std::string _server_address, std::string _gui_address) :
        gui_port(_gui_port),
        player_name(std::move(_player_name)),
        client_port(_client_port),
        server_port(_server_port),
        server_address(std::move(_server_address)),
        gui_address(std::move(_gui_address)) {}

    port_t getGuiPort() const;

    const std::string &getPlayerName() const;

    port_t getClientPort() const;

    port_t getServerPort() const;
};

class ArgumentsBuilder {
private:
    using port_t = uint16_t;

    port_t gui_port;
    std::string player_name;
    port_t client_port;
    port_t server_port;
    std::string server_address;
    std::string gui_address;

public:
    ArgumentsBuilder() = default;

    ArgumentsBuilder setGuiPort(port_t guiPort);

    ArgumentsBuilder setPlayerName(const std::string &playerName);

    ArgumentsBuilder setClientPort(port_t clientPort);

    ArgumentsBuilder setServerPort(port_t serverPort);

    ArgumentsBuilder setServerAddress(const std::string &serverAddress);

    ArgumentsBuilder setGuiAddress(const std::string &guiAddress);

    Arguments build();
};

Arguments parse_arguments(int argc, char *argv[]);


#endif //SIK_BOMBERMAN_ARGUMENTS_H
