#include "ServerGameState.h"

static void initialize_block_positions(ServerGameState &game_state, std::vector<std::shared_ptr<Event>> &events) {
    game_state.blocks.clear();

    for (size_t i = 0; i < game_state.initial_blocks_count; i++) {
        uint16_t x_position = (uint16_t) (game_state.randomizer.get_number() % game_state.size_x.get_num());
        uint16_t y_position = (uint16_t) (game_state.randomizer.get_number() % game_state.size_y.get_num());

        Position position{x_position, y_position};

        if (!game_state.blocks.contains(position)) {
            std::shared_ptr<Event> event = std::make_shared<BlockPlacedEvent>(position);

            game_state.blocks.insert(position);
            events.push_back(event);
        }
    }
}

static void initialize_player_positions(ServerGameState &game_state, std::vector<std::shared_ptr<Event>> &events) {
    game_state.player_positions.get_map().clear();

    for (auto &[key, value] : game_state.players.get_map()) {
        uint16_t x_position = (uint16_t) (game_state.randomizer.get_number() % game_state.size_x.get_num());
        uint16_t y_position = (uint16_t) (game_state.randomizer.get_number() % game_state.size_y.get_num());

        player_id_t player_id = key;
        Position position{x_position, y_position};
        std::shared_ptr<Event> event = std::make_shared<PlayerMovedEvent>(player_id, position);

        game_state.player_positions.get_map().insert({player_id, position});
        events.push_back(event);
    }
}

static void initialize_player_scores(ServerGameState &game_state) {
    game_state.scores.get_map().clear();

    for (auto &[key, value] : game_state.players.get_map()) {
        game_state.scores.get_map().insert({key, 0});
    }
}

static void reset_game_turns(ServerGameState &game_state) {
    game_state.game_turns.clear();
}

std::vector<std::shared_ptr<Event>> ServerGameState::initialize_new_game() {
    std::vector<std::shared_ptr<Event>> events;

    turn_number = 0;
    current_player_id = 0;
    current_bomb_id = 0;

    reset_game_turns(*this);
    initialize_player_scores(*this);
    initialize_player_positions(*this, events);
    initialize_block_positions(*this, events);

    return events;
}

static void update_players_scores(ServerGameState &game_state){
    for (auto &player_id : game_state.robots_destroyed_in_turn) {
        game_state.scores.get_map().at(player_id).get_num()++;
    }
}

bomb_id_t ServerGameState::get_next_bomb_id() {
    return {current_bomb_id++};
}

player_id_t ServerGameState::get_next_player_id() {
    return {current_player_id++};
}

void ServerGameState::update_after_turn() {
    turn_number++;
    update_players_scores(*this);
}

void ServerGameState::reset_turn_data() {
    blocks_destroyed_in_turn.clear();
    robots_destroyed_in_turn.clear();
}

void ServerGameState::update_bomb_timers() {
    for (auto &[key, bomb] : bombs) {
        bomb.dec_timer();
    }
}

HelloMessage ServerGameState::to_hello_message() {
    return HelloMessage{
            server_name,
            players_count,
            size_x,
            size_y,
            game_length,
            explosion_radius,
            bomb_timer,
    };
}

void ServerGameState::reset_game_state() {
    current_players_count = 0;
    players.get_map().clear();
    blocks.clear();
    bombs.clear();
    player_positions.get_map().clear();
    scores.get_map().clear();

    turn_number = 0;
    game_turns.clear();

    current_player_id = 0;
    current_bomb_id = 0;

    blocks_destroyed_in_turn.clear();
    robots_destroyed_in_turn.clear();
}

