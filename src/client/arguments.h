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

public:
    Arguments(port_t _gui_port, std::string _player_name, port_t _client_port, port_t _server_port) :
        gui_port(_gui_port),
        player_name(std::move(_player_name)),
        client_port(_client_port),
        server_port(_server_port) {}

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

public:
    ArgumentsBuilder() = default;

    void setGuiPort(port_t guiPort);

    void setPlayerName(const std::string &playerName);

    void setClientPort(port_t clientPort);

    void setServerPort(port_t serverPort);

    Arguments build();
};

Arguments parse_arguments(int argc, char *argv[]);


#endif //SIK_BOMBERMAN_ARGUMENTS_H
