#include "deserialization.h"
#include "boost/endian/conversion.hpp"

[[maybe_unused]] StringSerializer string_to_string_serializer(std::string &string) {
    return {string};
}

std::shared_ptr<InputMessage> string_to_input_message(std::string &string) {
    size_t len = string.size();
    if (len < 1)
        throw DeserializationException{};

    switch (string[0]) {
        case 0:
            if (len > 1) throw DeserializationException{};
            return std::make_shared<PlaceBombGui>();
        case 1:
            if (len > 1) throw DeserializationException{};
            return std::make_shared<PlaceBlockGui>();
        case 2:
            if (len > 2) throw DeserializationException{};
            return std::make_shared<MoveGui>(string[1]);
        default:
            throw DeserializationException{};
    }
}

static StringSerializer tcp_deserialize_to_string_serializer(ServerConnector &server_connector) {
    std::string message = server_connector.receive_message(1);
    assert(message.size() == 1);

    std::string string = server_connector.receive_message(static_cast<size_t>(message[0]));
    return {string};
}

static StringSerializer tcp_client_connector_deserialize_to_string_serializer(ClientConnector &client_connector) {
    std::string message = client_connector.receive_message(1);
    assert(message.size() == 1);

    std::string string = client_connector.receive_message(static_cast<size_t>(message[0]));
    return {string};
}

static UINT8Serializer tcp_deserialize_to_uint8_serializer(ServerConnector &server_connector) {
    std::string number = server_connector.receive_message(1);
    assert(number.size() == 1);

    return {static_cast<uint8_t>(number[0])};
}

static UINT16Serializer tcp_deserialize_to_uint16_serializer(ServerConnector &server_connector) {
    std::string number = server_connector.receive_message(2);
    assert(number.size() == 2);

    return {boost::endian::big_to_native(*((uint16_t*) number.data()))};
}

static UINT32Serializer tcp_deserialize_to_uint32_serializer(ServerConnector &server_connector) {
    std::string number = server_connector.receive_message(4);
    assert(number.size() == 4);

    return {boost::endian::big_to_native(*((uint32_t*) number.data()))};
}

static Player tcp_deserialize_to_player_serializer(ServerConnector &server_connector) {
    StringSerializer name = tcp_deserialize_to_string_serializer(server_connector);
    StringSerializer address = tcp_deserialize_to_string_serializer(server_connector);

    return {name, address};
}

static Position tcp_deserialize_to_position_serializer (ServerConnector &server_connector) {
    UINT16Serializer x = tcp_deserialize_to_uint16_serializer(server_connector);
    UINT16Serializer y = tcp_deserialize_to_uint16_serializer(server_connector);

    return {x, y};
}

static Direction tcp_client_connector_deserialize_to_direction(ClientConnector &client_connector) {
    std::string message = client_connector.receive_message(1);
    assert(message.size() == 1);

    return {static_cast<uint8_t>(message[0])};
}

static MapSerializer<Player> tcp_deserialize_to_player_map_serializer(ServerConnector &server_connector) {
    std::string number = server_connector.receive_message(4);
    assert(number.size() == 4);
    uint32_t true_number = boost::endian::big_to_native(*((uint32_t*) number.data()));

    std::map<UINT8Serializer, Player> map;

    for (size_t i = 0; i < true_number; i++) {
        UINT8Serializer key = tcp_deserialize_to_uint8_serializer(server_connector);
        Player player = tcp_deserialize_to_player_serializer(server_connector);

        map.insert({key, player});
    }

    return {map};
}

static MapSerializer<UINT32Serializer> tcp_deserialize_to_uint32_map_serializer(ServerConnector &server_connector) {
    std::string number = server_connector.receive_message(4);
    assert(number.size() == 4);
    uint32_t true_number = boost::endian::big_to_native(*((uint32_t*) number.data()));

    std::map<UINT8Serializer, UINT32Serializer> map;

    for (size_t i = 0; i < true_number; i++) {
        UINT8Serializer key = tcp_deserialize_to_uint8_serializer(server_connector);
        UINT32Serializer value = tcp_deserialize_to_uint32_serializer(server_connector);

        map.insert({key, value});
    }

    return {map};
}

static ListSerializer<UINT8Serializer> tcp_deserialize_to_uint8_list_serializer(ServerConnector &server_connector) {
    std::string number = server_connector.receive_message(4);
    assert(number.size() == 4);
    uint32_t true_number = boost::endian::big_to_native(*((uint32_t*) number.data()));

    std::vector<UINT8Serializer> list;

    for (size_t i = 0; i < true_number; i++) {
        UINT8Serializer element = tcp_deserialize_to_uint8_serializer(server_connector);
        list.push_back(element);
    }

    return {list};
}

static ListSerializer<Position> tcp_deserialize_to_position_list_serializer(ServerConnector &server_connector) {
    std::string number = server_connector.receive_message(4);
    assert(number.size() == 4);
    uint32_t true_number = boost::endian::big_to_native(*((uint32_t*) number.data()));

    std::vector<Position> list;

    for (size_t i = 0; i < true_number; i++) {
        Position element = tcp_deserialize_to_position_serializer(server_connector);
        list.push_back(element);
    }

    return {list};
}

static std::shared_ptr<BombPlacedEvent> tcp_deserialize_to_bomb_placed_event(ServerConnector &server_connector) {
    UINT32Serializer bomb_id = tcp_deserialize_to_uint32_serializer(server_connector);
    Position position = tcp_deserialize_to_position_serializer(server_connector);

    return std::make_shared<BombPlacedEvent>(bomb_id, position);
}


static std::shared_ptr<BombExplodedEvent> tcp_deserialize_to_bomb_exploded_event(ServerConnector &server_connector) {
    UINT32Serializer bomb_id = tcp_deserialize_to_uint32_serializer(server_connector);
    ListSerializer<player_id_t> robots_destroyed = tcp_deserialize_to_uint8_list_serializer(server_connector);
    ListSerializer<Position> blocks_destroyed = tcp_deserialize_to_position_list_serializer(server_connector);

    return std::make_shared<BombExplodedEvent>(bomb_id, robots_destroyed, blocks_destroyed);
}

static std::shared_ptr<PlayerMovedEvent> tcp_deserialize_to_player_moved_event(ServerConnector &server_connector) {
    UINT8Serializer player_id = tcp_deserialize_to_uint8_serializer(server_connector);
   Position position = tcp_deserialize_to_position_serializer(server_connector);

    return std::make_shared<PlayerMovedEvent>(player_id, position);
}

static std::shared_ptr<BlockPlacedEvent> tcp_deserialize_to_block_placed_event(ServerConnector &server_connector) {
    Position position = tcp_deserialize_to_position_serializer(server_connector);

    return std::make_shared<BlockPlacedEvent>(position);
}

static std::shared_ptr<Event> tcp_deserialize_to_event_message(ServerConnector &server_connector) {
    std::string message_id = server_connector.receive_message(1);
    assert(message_id.size() == 1);

    switch (message_id[0]) {
        case 0:
            return tcp_deserialize_to_bomb_placed_event(server_connector);
        case 1:
            return tcp_deserialize_to_bomb_exploded_event(server_connector);
        case 2:
            return  tcp_deserialize_to_player_moved_event(server_connector);
        case 3:
            return tcp_deserialize_to_block_placed_event(server_connector);
        default:
            throw DeserializationException{};
    }
}

static std::vector<std::shared_ptr<Event>> tcp_deserialize_to_event_vector(ServerConnector &server_connector) {
    std::string number = server_connector.receive_message(4);
    assert(number.size() == 4);
    uint32_t true_number = boost::endian::big_to_native(*((uint32_t*) number.data()));

    std::vector<std::shared_ptr<Event>> list;

    for (size_t i = 0; i < true_number; i++) {
        std::shared_ptr<Event> element = tcp_deserialize_to_event_message(server_connector);
        list.push_back(element);
    }

    return list;
}

HelloMessage tcp_deserialize_to_hello_message(ServerConnector &server_connector) {
    StringSerializer server_name = tcp_deserialize_to_string_serializer(server_connector);
    UINT8Serializer players_count = tcp_deserialize_to_uint8_serializer(server_connector);
    UINT16Serializer size_x = tcp_deserialize_to_uint16_serializer(server_connector);
    UINT16Serializer size_y = tcp_deserialize_to_uint16_serializer(server_connector);
    UINT16Serializer game_length = tcp_deserialize_to_uint16_serializer(server_connector);
    UINT16Serializer explosion_radius = tcp_deserialize_to_uint16_serializer(server_connector);
    UINT16Serializer bomb_timer = tcp_deserialize_to_uint16_serializer(server_connector);

    return {server_name, players_count, size_x, size_y, game_length, explosion_radius, bomb_timer};
}

AcceptedPlayerMessage tcp_deserialize_to_accepted_player_message(ServerConnector &server_connector) {
    player_id_t player_id = tcp_deserialize_to_uint8_serializer(server_connector);
    Player player = tcp_deserialize_to_player_serializer(server_connector);

    return {player_id, player};
}

GameStartedMessage tcp_deserialize_to_game_started_message(ServerConnector &server_connector) {
    MapSerializer<Player> players = tcp_deserialize_to_player_map_serializer(server_connector);

    return {players};
}

TurnMessage tcp_deserialize_to_turn_message(ServerConnector &server_connector) {
    UINT16Serializer turn = tcp_deserialize_to_uint16_serializer(server_connector);
    std::vector<std::shared_ptr<Event>> events = tcp_deserialize_to_event_vector(server_connector);

    return {turn, events};
}

GameEndedMessage tcp_deserialize_to_game_ended_message(ServerConnector &server_connector) {
    MapSerializer<score_t> scores = tcp_deserialize_to_uint32_map_serializer(server_connector);

    return {scores};
}

JoinServer tcp_deserialize_to_join_server_message(ClientConnector &client_connector) {
    StringSerializer name = tcp_client_connector_deserialize_to_string_serializer(client_connector);

    return {name};
}

PlaceBombServer tcp_deserialize_to_place_bomb_message() {
    return {};
}

PlaceBlockServer tcp_deserialize_to_place_block_message() {
    return {};
}

MoveServer tcp_deserialize_to_move_server_message(ClientConnector &client_connector) {
    Direction direction = tcp_client_connector_deserialize_to_direction(client_connector);

    return {direction};
}