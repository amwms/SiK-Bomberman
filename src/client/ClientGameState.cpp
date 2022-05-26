#include "ClientGameState.h"

static void update_players(ClientGameState &game_state){
    for (auto &player_id : game_state.robots_destroyed_in_turn) {
        game_state.scores.get_map().at(player_id).get_num()++;
    }
}

static void update_blocks(ClientGameState &game_state){
    for (auto &el : game_state.blocks_destroyed_in_turn) {
        game_state.blocks.erase(el);
    }
}

void ClientGameState::update_bomb_timers() {
    for (auto &[key, bomb] : bombs) {
        bomb.dec_timer();
    }
}

void ClientGameState::update_after_turn() {
    update_blocks(*this);
    update_players(*this);
}

void ClientGameState::reset_turn_data() {
    blocks_destroyed_in_turn.clear();
    explosions.clear();
    robots_destroyed_in_turn.clear();
}

template<class T>
ListSerializer<T> cast_set_to_list(std::set<T> &to_cast) {
    std::vector<T> list;

    for (auto &el : to_cast) {
        list.push_back(el);
    }

    return {list};
}

template<class T>
ListSerializer<T> cast_map_to_list(auto &to_cast) {
    std::vector<T> list;

    for (auto &[key, value] : to_cast) {
        list.push_back(value);
    }

    return {list};
}

LobbyMessage ClientGameState::to_lobby_massage() {
    return LobbyMessage{
        server_name,
        players_count,
        size_x,
        size_y,
        game_length,
        explosion_radius,
        bomb_timer,
        players
    };
}

GameMessage ClientGameState::to_game_message() {
    return GameMessage{
        server_name,
        size_x,
        size_y,
        game_length,
        turn,
        players,
        player_positions,
        cast_set_to_list<Position>(blocks),
        cast_map_to_list<Bomb>(bombs),
        cast_set_to_list<Position>(explosions),
        scores
    };
}
