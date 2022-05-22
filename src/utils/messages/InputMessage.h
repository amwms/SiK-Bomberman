#ifndef SIK_BOMBERMAN_INPUTMESSAGE_H
#define SIK_BOMBERMAN_INPUTMESSAGE_H

#include "../serializer.h"
#include "../utils.h"

class InputMessage : public Serializer {
protected:
    uint8_t message_id;

public:
    InputMessage(uint8_t _message_id) : message_id(_message_id) {}

    std::string serialize() override;
};

class PlaceBombGui: public InputMessage {
    static const uint8_t PLACE_BOMB_MESSAGE_ID = 0;

public:
    PlaceBombGui() : InputMessage(PLACE_BOMB_MESSAGE_ID) {}
};

class PlaceBlockGui : public InputMessage {
    static const uint8_t PLACE_BLOCK_MESSAGE_ID = 1;

public:
    PlaceBlockGui() : InputMessage(PLACE_BLOCK_MESSAGE_ID) {}
};

class MoveGui : public InputMessage {
    static const uint8_t MOVE_MESSAGE_ID = 2;
    Direction direction;

public:
    MoveGui(uint8_t _direction_id) : InputMessage(MOVE_MESSAGE_ID), direction(_direction_id) {}

    std::string serialize() override;
};

#endif //SIK_BOMBERMAN_INPUTMESSAGE_H
