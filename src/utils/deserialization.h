#ifndef SIK_BOMBERMAN_DESERIALIZATION_H
#define SIK_BOMBERMAN_DESERIALIZATION_H

#include <memory>
#include "serializer.h"
#include "messages/InputMessage.h"
#include "messages/ServerMessage.h"
#include "../client/ServerConnector.h"

class DeserializationException : public std::exception {};

std::shared_ptr<InputMessage> string_to_input_message(std::string &string);

HelloMessage tcp_deserialize_to_hello_message(ServerConnector &server_connector);

AcceptedPlayerMessage tcp_deserialize_to_accepted_player_message(ServerConnector &server_connector);

GameStartedMessage tcp_deserialize_to_game_started_message(ServerConnector &server_connector);

TurnMessage tcp_deserialize_to_turn_message(ServerConnector &server_connector);

GameEndedMessage tcp_deserialize_to_game_ended_message(ServerConnector &server_connector);

#endif //SIK_BOMBERMAN_DESERIALIZATION_H
