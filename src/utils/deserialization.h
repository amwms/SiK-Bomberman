#ifndef SIK_BOMBERMAN_DESERIALIZATION_H
#define SIK_BOMBERMAN_DESERIALIZATION_H

#include <memory>
#include "serializer.h"
#include "messages/InputMessage.h"

class DeserializationException : public std::exception {};

StringSerializer string_to_string_serializer(std::string &string);

std::shared_ptr<InputMessage> string_to_input_message(std::string &string);

#endif //SIK_BOMBERMAN_DESERIALIZATION_H
