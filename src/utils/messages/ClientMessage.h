#ifndef SIK_BOMBERMAN_CLIENTMESSAGE_H
#define SIK_BOMBERMAN_CLIENTMESSAGE_H

#include "../serializer.h"
#include "../utils.h"

class ClientMessage : public Serializer {
protected:
    uint8_t message_id;

public:
    ClientMessage(uint8_t _message_id) : message_id(_message_id) {}

    std::string serialize() const override;

    uint8_t get_message_id();
};

class JoinServer: public ClientMessage {
    static const uint8_t JOIN_SERVER_MESSAGE_ID = 0;
    StringSerializer name;

public:
    JoinServer(const std::string &_name) : ClientMessage(JOIN_SERVER_MESSAGE_ID), name(_name) {}
    JoinServer(const StringSerializer &_name) : ClientMessage(JOIN_SERVER_MESSAGE_ID), name(_name) {}

    std::string serialize() const override;

    std::string get_name();
};

class PlaceBombServer: public ClientMessage {
    static const uint8_t PLACE_BOMB_MESSAGE_ID = 1;

public:
    PlaceBombServer() : ClientMessage(PLACE_BOMB_MESSAGE_ID) {}
};

class PlaceBlockServer: public ClientMessage {
    static const uint8_t PLACE_BLOCK_MESSAGE_ID = 2;

public:
    PlaceBlockServer() : ClientMessage(PLACE_BLOCK_MESSAGE_ID) {}
};

class MoveServer: public ClientMessage {
    static const uint8_t MOVE_MESSAGE_ID = 3;
    Direction direction;

public:
    MoveServer(uint8_t _direction) : ClientMessage(MOVE_MESSAGE_ID), direction(_direction) {}
    MoveServer(Direction &_direction) : ClientMessage(MOVE_MESSAGE_ID), direction(_direction) {}

    std::string serialize() const override;

    const Direction &get_direction() const;
};


#endif //SIK_BOMBERMAN_CLIENTMESSAGE_H
