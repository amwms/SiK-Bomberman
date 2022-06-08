#include "ServerArguments.h"
#include <boost/program_options.hpp>
#include <iostream>
#include <chrono>

namespace po = boost::program_options;

//-b, --bomb-timer <u16>
//-c, --players-count <u8>
//-d, --turn-duration <u64, milisekundy>
//-e, --explosion-radius <u16>
//-h, --help               Wypisuje jak używać programu
//-k, --initial-blocks <u16>
//-l, --game-length <u16>
//-n, --server-name <String>
//-p, --port <u16>
//-s, --seed <u32, parametr opcjonalny>
//-x, --size-x <u16>
//-y, --size-y <u16>

#define BOMB_TIMER "bomb-timer"
#define PLAYERS_COUNT "players-count"
#define TURN_DURATION "turn duration"
#define EXPLOSION_RADIUS "explosion-radius"
#define HELP "help"
#define INITIAL_BLOCKS "initial-blocks"
#define GAME_LENGTH "game-length"
#define SERVER_NAME "server-name"
#define PORT "port"
#define SEED "seed"
#define SIZE_X "size-x"
#define SIZE_Y "size-y"


static void print_exit(auto &message) {
    std::cerr << message << "\n";
    exit(1);
}

static ServerArguments create_arguments_from_map(po::variables_map &variables_map) {
    uint32_t seed;
    if (variables_map.find(SEED) == variables_map.end()) {
        seed = (uint32_t) std::chrono::system_clock::now().time_since_epoch().count();
    }
    else {
        seed = variables_map[SEED].as<uint32_t>();
    }

    return ServerArguments{variables_map[BOMB_TIMER].as<uint16_t>(),
                           variables_map[PLAYERS_COUNT].as<uint8_t>(),
                           variables_map[TURN_DURATION].as<uint64_t>(),
                           variables_map[EXPLOSION_RADIUS].as<uint16_t>(),
                           variables_map[INITIAL_BLOCKS].as<uint16_t>(),
                           variables_map[GAME_LENGTH].as<uint16_t>(),
                           variables_map[SERVER_NAME].as<std::string>(),
                           variables_map[PORT].as<uint16_t>(),
                           seed,
                           variables_map[SIZE_X].as<uint16_t>(),
                           variables_map[SIZE_Y].as<uint16_t>()};
}

ServerArguments parse_server_arguments(int argc, char *argv[]) {
    // declare the supported options
    po::options_description desc("Program options");
    po::positional_options_description p;
    p.add("redundant", -1);

    desc.add_options()
            (BOMB_TIMER",b", po::value<uint16_t>()->required(), "<u16>")
            (PLAYERS_COUNT",c", po::value<uint8_t>()->required(), "<u8>")
            (TURN_DURATION",d", po::value<uint64_t>()->required(), "<u64, milisekundy>")
            (EXPLOSION_RADIUS",e", po::value<uint16_t>()->required(), "<u16>")
            (HELP",h", "Wypisuje jak używać programu")
            (INITIAL_BLOCKS",k", po::value<uint16_t>()->required(), "<u16>")
            (GAME_LENGTH",l", po::value<uint16_t>()->required(), "<u16>")
            (SERVER_NAME",n", po::value<std::string>()->required(), "<string>")
            (PORT",p", po::value<uint16_t>()->required(), "<u16>")
            (SEED",s", po::value<uint32_t>(), "<u32, parametr opcjonalny>")
            (SIZE_X",x", po::value<uint16_t>()->required(), "<u16>")
            (SIZE_Y",y", po::value<uint16_t>()->required(), "<u16>");

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

    return create_arguments_from_map(variables_map);
}
