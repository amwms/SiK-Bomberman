#ifndef SIK_BOMBERMAN_EVENT_H
#define SIK_BOMBERMAN_EVENT_H

#include "serializer.h"
#include "utils.h"

class Event : public Serializer {
protected:
    uint8_t message_id;

public:
    Event(uint8_t _message_id) : message_id(_message_id) {}
};

class BombPlacedEvent : public Event {
    static const uint8_t BOMB_PLACED_EVENT_ID = 0;
    bomb_id_t bomb_id;
    Position position;

public:
    BombPlacedEvent(bomb_id_t &_bomb_id, Position &_position) :
        Event(BOMB_PLACED_EVENT_ID), bomb_id(_bomb_id), position(_position) {}

    std::string serialize() override;
};

class BombExplodedEvent : public Event {
    static const uint8_t BOMB_EXPLODED_EVENT_ID = 0;
    bomb_id_t bomb_id;
    ListSerializer<player_id_t> robots_destroyed;
    ListSerializer<Position> blocks_destroyed;

public:
    BombExplodedEvent(const bomb_id_t &_bomb_id, ListSerializer<player_id_t> &_robots_destroyed,
                      ListSerializer<Position> &_blocks_destroyed) :
            Event(BOMB_EXPLODED_EVENT_ID), bomb_id(_bomb_id),
            robots_destroyed(_robots_destroyed), blocks_destroyed(_blocks_destroyed) {}

    std::string serialize() override;
};

class PlayerMovedEvent : public Event {
    static const uint8_t PLAYER_MOVED_EVENT_ID = 0;
    bomb_id_t bomb_id;
    Position position;

public:
    PlayerMovedEvent(const bomb_id_t &_bomb_id, const Position &_position) :
            Event(PLAYER_MOVED_EVENT_ID), bomb_id(_bomb_id), position(_position) {}

    std::string serialize() override;
};

class BlockPlacedEvent : public Event {
    static const uint8_t BLOCK_PLACED_EVENT_ID = 0;
    Position position;

public:
    BlockPlacedEvent(const Position &_position) : Event(BLOCK_PLACED_EVENT_ID), position(_position) {}

    std::string serialize() override;
};


#endif //SIK_BOMBERMAN_EVENT_H
