#ifndef SIK_BOMBERMAN_DRAWMESSAGE_H
#define SIK_BOMBERMAN_DRAWMESSAGE_H

#include "../serializer.h"
#include "../utils.h"

class LobbyMessage : public Serializer {
    uint8_t message_id = 0;
    StringSerializer server_name;
    uint8_t players_count;
    UINT16Serializer size_x;
    UINT16Serializer size_y;
    UINT16Serializer game_length;
    UINT16Serializer explosion_radius;
    UINT16Serializer bomb_timer;
    MapSerializer<Player> players;

public:
    LobbyMessage(const std::string &serverName, uint8_t playersCount, uint16_t sizeX,
                 uint16_t sizeY, uint16_t gameLength, uint16_t explosionRadius, uint16_t bombTimer,
                 const MapSerializer<Player> &players) : server_name(serverName),
                                                         players_count(playersCount), size_x(sizeX),
                                                         size_y(sizeY), game_length(gameLength),
                                                         explosion_radius(explosionRadius),
                                                         bomb_timer(bombTimer), players(players) {}

    std::string serialize() override;
};

class GameMessage : public Serializer {
    uint8_t message_id = 1;
    StringSerializer server_name;
    UINT16Serializer size_x;
    UINT16Serializer size_y;
    UINT16Serializer game_length;
    UINT16Serializer turn;
    MapSerializer<Player> players;
    MapSerializer<Position> player_positions;
    ListSerializer<Position> blocks;
    ListSerializer<Bomb> bombs;
    ListSerializer<Position> explosions;
    MapSerializer<score_t> scores;

public:
    GameMessage(const std::string &serverName, uint16_t sizeX,
                uint16_t sizeY, uint16_t gameLength,
                uint16_t turn, const MapSerializer<Player> &players,
                const MapSerializer<Position> &playerPositions, const ListSerializer<Position> &blocks,
                const ListSerializer<Bomb> &bombs, const ListSerializer<Position> &explosions,
                const MapSerializer<score_t> &scores) : server_name(serverName),
                                                        size_x(sizeX), size_y(sizeY), game_length(gameLength),
                                                        turn(turn), players(players),
                                                        player_positions(playerPositions), blocks(blocks),
                                                        bombs(bombs), explosions(explosions), scores(scores) {}

    std::string serialize() override;
};

#endif //SIK_BOMBERMAN_DRAWMESSAGE_H
