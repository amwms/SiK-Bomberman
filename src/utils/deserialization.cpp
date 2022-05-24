#include "deserialization.h"

StringSerializer string_to_string_serializer(std::string &string) {
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

StringSerializer tcp_deserialize_to_string_serializer(ServerConnector &server_connector) {
    std::string message = server_connector.receive_message(1);
    assert(message.size() == 1);

    std::string string = server_connector.receive_message(static_cast<size_t>(message[0]));
    return {string};
}

UINT8Serializer tcp_deserialize_to_uint8_serializer(ServerConnector &server_connector) {
    std::string number = server_connector.receive_message(1);
    assert(number.size() == 1);

    return {static_cast<uint8_t>(number[0])};
}

UINT16Serializer tcp_deserialize_to_uint16_serializer(ServerConnector &server_connector) {
    std::string number = server_connector.receive_message(2);
    assert(number.size() == 2);

    return {*((uint16_t*) number.data())};
}

UINT32Serializer tcp_deserialize_to_uint32_serializer(ServerConnector &server_connector) {
    std::string number = server_connector.receive_message(4);
    assert(number.size() == 4);

    return {*((uint32_t*) number.data())};
}

Player tcp_deserialize_to_player_serializer(ServerConnector &server_connector) {
    StringSerializer name = tcp_deserialize_to_string_serializer(server_connector);
    StringSerializer address = tcp_deserialize_to_string_serializer(server_connector);

    return {name, address};
}

MapSerializer<Player> tcp_deserialize_to_map_serializer(ServerConnector &server_connector) {
    std::string number = server_connector.receive_message(4);
    assert(number.size() == 4);
    uint32_t true_number = {*((uint32_t*) number.data())};

    std::map<UINT8Serializer, Player> map;

    for (size_t i = 0; i < true_number; i++) {
        UINT8Serializer key = tcp_deserialize_to_uint8_serializer(server_connector);
        Player player = tcp_deserialize_to_player_serializer(server_connector);

        map.insert({key, player});
    }

    return {map};
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
    MapSerializer<Player> players = tcp_deserialize_to_map_serializer(server_connector);

    return {players};
}
