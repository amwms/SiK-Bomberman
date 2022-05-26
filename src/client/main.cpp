#include <iostream>
#include "arguments.h"
#include "../utils/serializer.h"
#include "../utils/utils.h"
#include "../utils/messages/DrawMessage.h"
#include "GuiConnector.h"
#include "GuiHandler.h"
#include "ServerHandler.h"

#define GUI_ADDRESS "gui-address"
#define HELP "help"
#define PLAYER_NAME "player-name"
#define PORT "port"
#define SERVER_ADDRESS "server-address"

int main(int argc, char *argv[]) {
    Arguments arguments = parse_arguments(argc, argv);

    try {
        // set needed variables for concurrency implementation
        std::mutex m;
        std::condition_variable cond;
        std::atomic<std::thread::id> val;

        std::function<void(void)> thread_callback = [&] {
            val = std::this_thread::get_id();
            cond.notify_all();
        };

        // set needed variables for network implementation
        boost::asio::io_context io_context;
        GuiConnector gui_connector{io_context, arguments.get_gui_address(), arguments.get_gui_port(),
                                   arguments.get_client_port()};
        ServerConnector server_connector{io_context, arguments.get_server_address(), arguments.get_server_port()};
        ClientGameState game_state{};

        GuiHandler gui_handler{server_connector, gui_connector, game_state, arguments.get_player_name(), thread_callback};
        ServerHandler server_handler{server_connector, gui_connector, game_state, thread_callback};

        std::thread gui_handler_thread(gui_handler);
        std::thread server_handler_thread(server_handler);

        // wait for gui_handler or server_handler to finish their work
        std::unique_lock<std::mutex> lock{m};
        cond.wait(lock, [&] { return val != std::thread::id{}; });
    }
    catch (std::exception &exception) {
        std::cerr << exception.what() << std::endl;
    }

    exit(1);
}
