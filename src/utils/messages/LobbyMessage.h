#ifndef SIK_BOMBERMAN_LOBBYMESSAGE_H
#define SIK_BOMBERMAN_LOBBYMESSAGE_H

#include <cstdint>
#include <string>
#include "../Player.h"
#include "../serializer.h"

using playerid_t = uint8_t;

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
                               const MapSerializer<Player> &players) : server_name({serverName}),
                                                                       players_count(playersCount), size_x(sizeX),
                                                                       size_y(sizeY), game_length(gameLength),
                                                                       explosion_radius(explosionRadius),
                                                                       bomb_timer(bombTimer), players(players) {}

    std::string serialize() override {
        std::string result;

        result += (char) message_id;
        result += server_name.serialize();
        result += (char) players_count;
        result += size_x.serialize();
        result += size_y.serialize();
        result += game_length.serialize();
        result += explosion_radius.serialize();
        result += bomb_timer.serialize();
        result += players.serialize();

        return result;
    }
};


#endif //SIK_BOMBERMAN_LOBBYMESSAGE_H
