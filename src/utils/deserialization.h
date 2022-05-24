#ifndef SIK_BOMBERMAN_DESERIALIZATION_H
#define SIK_BOMBERMAN_DESERIALIZATION_H

#include <memory>
#include "serializer.h"
#include "messages/InputMessage.h"
#include "messages/ServerMessage.h"
#include "../client/ServerConnector.h"

class DeserializationException : public std::exception {};

StringSerializer string_to_string_serializer(std::string &string);

std::shared_ptr<InputMessage> string_to_input_message(std::string &string);

StringSerializer tcp_deserialize_to_string_serializer(ServerConnector &server_connector);

UINT8Serializer tcp_deserialize_to_uint8_serializer(ServerConnector &server_connector);
UINT16Serializer tcp_deserialize_to_uint16_serializer(ServerConnector &server_connector);
UINT32Serializer tcp_deserialize_to_uint32_serializer(ServerConnector &server_connector);

HelloMessage tcp_deserialize_to_hello_message(ServerConnector &server_connector);

AcceptedPlayerMessage tcp_deserialize_to_accepted_player_message(ServerConnector &server_connector);

GameStartedMessage tcp_deserialize_to_game_started_message(ServerConnector &server_connector);

#endif //SIK_BOMBERMAN_DESERIALIZATION_H
