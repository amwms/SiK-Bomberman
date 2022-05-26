#include "event.h"

std::string BombPlacedEvent::serialize() const {
    std::string result;

    result += static_cast<char>(message_id);
    result += bomb_id.serialize();
    result += position.serialize();

    return result;
}

void BombPlacedEvent::handle(ClientGameState &game_state) {
    game_state.bombs.insert({bomb_id, {position, game_state.bomb_timer}});
}

std::string BombExplodedEvent::serialize() const {
    std::string result;

    result += static_cast<char>(message_id);
    result += bomb_id.serialize();
    result += robots_destroyed.serialize();
    result += blocks_destroyed.serialize();

    return result;
}

static bool out_of_bounds(ClientGameState &game_state, Position &position) {
    return position.get_x() >= game_state.size_x.get_num()
        || position.get_y() >= game_state.size_y.get_num();
}

static void count_explosions(ClientGameState &game_state, bomb_id_t &bomb_id) {
    Position start_position = game_state.bombs.at(bomb_id).get_position();

    // up from
    Position current_position = start_position;
    game_state.explosions.insert(current_position);
    uint16_t radius = 0;

    while (game_state.blocks.find(current_position) == game_state.blocks.end()
            && !out_of_bounds(game_state, current_position)
            && radius < game_state.explosion_radius.get_num()) {
        current_position.inc_y();

        if (!out_of_bounds(game_state, current_position))
            game_state.explosions.insert(current_position);

        radius++;
    }

    // down from bomb
    current_position = start_position;
    radius = 0;

    while (game_state.blocks.find(current_position) == game_state.blocks.end()
           && !out_of_bounds(game_state, current_position)
           && radius < game_state.explosion_radius.get_num()) {
        current_position.dec_y();

        if (!out_of_bounds(game_state, current_position))
            game_state.explosions.insert(current_position);

        radius++;
    }

    // left from bomb
    current_position = start_position;
    radius = 0;

    while (game_state.blocks.find(current_position) == game_state.blocks.end()
           && !out_of_bounds(game_state, current_position)
           && radius < game_state.explosion_radius.get_num()) {
        current_position.dec_x();

        if (!out_of_bounds(game_state, current_position))
            game_state.explosions.insert(current_position);

        radius++;
    }

    // right from bomb
    current_position = start_position;
    radius = 0;

    while (game_state.blocks.find(current_position) == game_state.blocks.end()
           && !out_of_bounds(game_state, current_position)
           && radius < game_state.explosion_radius.get_num()) {
        current_position.inc_x();

        if (!out_of_bounds(game_state, current_position))
            game_state.explosions.insert(current_position);

        radius++;
    }
}

void BombExplodedEvent::handle(ClientGameState &game_state) {
    for (auto &robot : robots_destroyed.get_list()) {
        game_state.robots_destroyed_in_turn.insert(robot);
    }

    for (auto &block : blocks_destroyed.get_list()) {
        game_state.blocks_destroyed_in_turn.insert(block);
    }

    count_explosions(game_state, bomb_id);
    game_state.bombs.erase(bomb_id);
}

std::string PlayerMovedEvent::serialize() const {
    std::string result;

    result += static_cast<char>(message_id);
    result += player_id.serialize();
    result += position.serialize();

    return result;
}

void PlayerMovedEvent::handle(ClientGameState &game_state) {
    game_state.player_positions.get_map().at(player_id) = position;
}

std::string BlockPlacedEvent::serialize() const {
    std::string result;

    result += static_cast<char>(message_id);
    result += position.serialize();

    return result;
}

void BlockPlacedEvent::handle(ClientGameState &game_state) {
    game_state.blocks.insert(position);
}
