#include <iostream>
#include "ServerGameHandler.h"

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

using namespace std::chrono;

using action_t = std::variant<JoinServer, PlaceBombServer, PlaceBlockServer, MoveServer>;

static bool out_of_bounds(ServerGameState &game_state, Position &position) {
    return position.get_x() >= game_state.size_x.get_num()
           || position.get_y() >= game_state.size_y.get_num();
}

static void update_destructions_per_bomb(ServerGameState &game_state, std::vector<Position> &blocks_destroyed,
                                         Position &current_position, std::set<Position> &explosions) {
    if (!out_of_bounds(game_state, current_position)) {
        auto block_it = game_state.blocks.find(current_position);
        explosions.insert(current_position);

        if (block_it != game_state.blocks.end()) {
            game_state.blocks_destroyed_in_turn.insert(*block_it);
            blocks_destroyed.push_back(*block_it);
        }
    }
}

static void count_explosions(ServerGameState &game_state, const bomb_id_t &bomb_id,
                             std::vector<Position> &blocks_destroyed, std::vector<player_id_t> &robots_destroyed) {
    Position start_position = game_state.bombs.at(bomb_id).get_position();
    std::set<Position> explosions;

    // up from
    Position current_position = start_position;
    uint16_t radius = 0;

    while (game_state.blocks.find(current_position) == game_state.blocks.end()
           && !out_of_bounds(game_state, current_position)
           && radius < game_state.explosion_radius.get_num()) {
        current_position.inc_y();
        update_destructions_per_bomb(game_state, blocks_destroyed, current_position, explosions);
        radius++;
    }

    // down from bomb
    current_position = start_position;
    radius = 0;

    while (game_state.blocks.find(current_position) == game_state.blocks.end()
           && !out_of_bounds(game_state, current_position)
           && radius < game_state.explosion_radius.get_num()) {
        current_position.dec_y();
        update_destructions_per_bomb(game_state, blocks_destroyed, current_position, explosions);
        radius++;
    }

    // left from bomb
    current_position = start_position;
    radius = 0;

    while (game_state.blocks.find(current_position) == game_state.blocks.end()
           && !out_of_bounds(game_state, current_position)
           && radius < game_state.explosion_radius.get_num()) {
        current_position.dec_x();
        update_destructions_per_bomb(game_state, blocks_destroyed, current_position, explosions);
        radius++;
    }

    // right from bomb
    current_position = start_position;
    radius = 0;

    while (game_state.blocks.find(current_position) == game_state.blocks.end()
           && !out_of_bounds(game_state, current_position)
           && radius < game_state.explosion_radius.get_num()) {
        current_position.inc_x();
        update_destructions_per_bomb(game_state, blocks_destroyed, current_position, explosions);
        radius++;
    }

    // find destroyed robots
    for (auto &[player_id, position] : game_state.player_positions.get_map()) {
        if (explosions.contains(position)) {
            robots_destroyed.push_back(player_id);
            game_state.robots_destroyed_in_turn.insert(player_id);
        }
    }
}

static std::shared_ptr<BombExplodedEvent> handle_one_explosion(ServerGameState &game_state, const bomb_id_t &bomb_id) {
    std::vector<Position> blocks_destroyed;
    std::vector<player_id_t> robots_destroyed;

    count_explosions(game_state, bomb_id, blocks_destroyed, robots_destroyed);

    ListSerializer<Position> blocks{blocks_destroyed};
    ListSerializer<player_id_t> robots{robots_destroyed};

    return std::make_shared<BombExplodedEvent>(bomb_id, robots, blocks);
}

static void update_blocks(ServerGameState &game_state){
    for (auto &el : game_state.blocks_destroyed_in_turn) {
        game_state.blocks.erase(el);
    }
}

static Position generate_random_position(ServerGameState &game_state) {
    uint16_t x_position = (uint16_t) game_state.randomizer.get_number() % game_state.size_x.get_num();
    uint16_t y_position = (uint16_t) game_state.randomizer.get_number() % game_state.size_y.get_num();

    return {x_position, y_position};
}

static Position count_new_position(Position &old_position, Direction &direction) {
    static int dx[] = {0, 1, 0, -1};
    static int dy[] = {1, 0, -1, 0};

    uint16_t y = old_position.get_y();
    uint16_t x = old_position.get_x();
    uint8_t direction_id = direction.get_direction_id();

    return {static_cast<uint16_t>(x + dx[direction_id]), static_cast<uint16_t>(y + dy[direction_id])};
}

static void handle_destroy_robot(ServerGameState &game_state, const player_id_t &player_id,
                                 std::vector<std::shared_ptr<Event>> &events) {
    auto player_position_it = game_state.player_positions.get_map().find(player_id);  // .at()

    if (player_position_it != game_state.player_positions.get_map().end()) {
        Position position = generate_random_position(game_state);
        player_position_it->second = position;

        std::shared_ptr<Event> event = std::make_shared<PlayerMovedEvent>(player_id, position);
        events.push_back(event);
    }
}

static void handle_explosions(ServerGameState &game_state, std::vector<std::shared_ptr<Event>> &events) {
    for (auto &[bomb_id, bomb] : game_state.bombs) {
        if (bomb.get_timer() == 0) {
            events.push_back(handle_one_explosion(game_state, bomb_id));
            game_state.bombs.erase(bomb_id);
        }
    }

    update_blocks(game_state);
}

static void handle_place_bomb(ServerGameState &game_state, [[maybe_unused]] PlaceBombServer &action,
                              const player_id_t &player_id, std::vector<std::shared_ptr<Event>> &events) {
    Position position = game_state.player_positions.get_map().at(player_id);
    bomb_id_t bomb_id = game_state.get_next_bomb_id();

    game_state.bombs.insert({bomb_id, {position, game_state.bomb_timer}});

    std::shared_ptr<Event> event = std::make_shared<BombPlacedEvent>(bomb_id, position);
    events.push_back(event);
}

static void handle_place_block(ServerGameState &game_state, [[maybe_unused]] PlaceBlockServer &action,
                               const player_id_t &player_id, std::vector<std::shared_ptr<Event>> &events) {
    Position position = game_state.player_positions.get_map().at(player_id);

    if (!game_state.blocks.contains(position)) {
        game_state.blocks.insert(position);

        std::shared_ptr<Event> event = std::make_shared<BlockPlacedEvent>(position);
        events.push_back(event);
    }
}

static void handle_move(ServerGameState &game_state, MoveServer &action, const player_id_t &player_id,
                        std::vector<std::shared_ptr<Event>> &events) {
    Position old_position = game_state.player_positions.get_map().at(player_id);
    Direction move_direction = action.get_direction();
    Position new_position = count_new_position(old_position, move_direction);

    if (!game_state.blocks.contains(new_position)) {
        game_state.player_positions.get_map().at(player_id) = new_position;

        std::shared_ptr<Event> event = std::make_shared<PlayerMovedEvent>(player_id, new_position);
        events.push_back(event);
    }
}

static void ignore_action([[maybe_unused]] auto &action) {}

void ServerGameHandler::clean_all_client_queues() {
    for (auto &client_handler : client_handlers) {
        auto queue = client_handler->client_receiving_queue->get_queue_no_mutex();
        while (!queue.empty()) {
            queue.pop();
        }
    }
}

action_t ServerGameHandler::get_last_action_in_queue(client_receiving_queue_t &receiving_queue) {
    action_t action = receiving_queue.front();
    receiving_queue.pop();

    while (!receiving_queue.empty()) {
        action_t temp = receiving_queue.front();

        std::visit(overloaded {
                [&](JoinServer &t) { ignore_action(t); },
                [&](PlaceBombServer &t) { action = t; },
                [&](PlaceBlockServer &t) { action = t; },
                [&](MoveServer &t) { action = t; },
        }, temp);
    }

    return action;
}

void ServerGameHandler::lock_all_receiving_queues_in_player_clients() {
    for (auto &client_handler : client_handlers) {
        client_handler->client_receiving_queue->lock_queue();
    }
}

void ServerGameHandler::unlock_all_receiving_queues_in_player_clients() {
    for (auto &client_handler : client_handlers) {
        client_handler->client_receiving_queue->unlock_queue();
    }
}

void ServerGameHandler::send_message(const std::basic_string<char> &message) {
    for (auto &client_handler : client_handlers) {
        client_handler->client_sending_queue->push(message);
    }
}

void ServerGameHandler::handle_game_turn() {
    lock_all_receiving_queues_in_player_clients();
    game_state.update_bomb_timers();

    std::vector<std::shared_ptr<Event>> events;
    handle_explosions(game_state, events);

    for (auto &client_handler : client_handlers) {
        auto receiving_queue = client_handler->client_receiving_queue->get_queue_no_mutex();
        if (client_handler->is_player() && !receiving_queue.empty()) {
            action_t value = get_last_action_in_queue(receiving_queue);

            player_id_t player_id = client_handler->get_player_id();
            bool is_robot_destroyed = game_state.robots_destroyed_in_turn.contains(player_id);

            if (!is_robot_destroyed) {
                std::visit(overloaded{
                        [&](JoinServer &action) { ignore_action(action); },
                        [&](PlaceBombServer &action) { handle_place_bomb(game_state, action, player_id, events); },
                        [&](PlaceBlockServer &action) { handle_place_block(game_state, action, player_id, events); },
                        [&](MoveServer &action) { handle_move(game_state, action, player_id, events); },
                }, value);
            }
        }
    }

    clean_all_client_queues();
    unlock_all_receiving_queues_in_player_clients();

    TurnMessage turn_message{game_state.turn_number, events};

    game_state.game_turns.push_back(turn_message);
    send_message(turn_message.serialize());

    // update after turn
    for (auto &player_id : game_state.robots_destroyed_in_turn) {
        handle_destroy_robot(game_state, player_id, events);
    }

    game_state.update_after_turn();
    game_state.reset_turn_data();
//    clean_all_client_queues();
//
//    unlock_all_queues_in_player_clients();
}

void ServerGameHandler::handle_join_server(JoinServer &action, ClientHandler &client_handler) {
    player_id_t new_player_id = game_state.get_next_player_id();
    std::string address = client_handler.client_connector->get_socket_address();
    std::string name = action.get_name();

    Player new_player{name, address};
    game_state.players.get_map().insert({new_player_id, new_player});
    game_state.current_players_count++;

    client_handler.set_player_id(new_player_id);

    AcceptedPlayerMessage accepted_player_message{new_player_id, new_player};
    send_message(accepted_player_message.serialize());
}

void ServerGameHandler::handle_new_joins() {
    for (auto &client_handler : client_handlers) {
        if (!client_handler->is_player()) {
            client_handler->client_receiving_queue->lock_queue();
            auto &queue = client_handler->client_receiving_queue->get_queue_no_mutex();

            if (!queue.empty()) {
                auto message = queue.front();
                queue.pop();

                std::visit(overloaded{
                        [&](JoinServer &action) { handle_join_server(action, *client_handler); },
                        [&](PlaceBombServer &action) { ignore_action(action); },
                        [&](PlaceBlockServer &action) { ignore_action(action); },
                        [&](MoveServer &action) { ignore_action(action); },
                }, message);
            }

            client_handler->client_receiving_queue->unlock_queue();
        }
    }
}

void ServerGameHandler::handle_lobby() {
    while (game_state.is_lobby && game_state.current_players_count < game_state.players_count.get_num()) {
        handle_new_joins();
        sleep(1);
    }

    game_state.is_lobby = false;
}

void ServerGameHandler::handle_game() {
    // GameStarted
    GameStartedMessage game_started_message{game_state.players};
    send_message(game_started_message.serialize());

    // Turns
    auto events = game_state.initialize_new_game();
    TurnMessage turn_0{0, events};
    game_state.game_turns.push_back(turn_0);
    send_message(turn_0.serialize());
    game_state.update_after_turn();
    game_state.reset_turn_data();

    uint turn_duration = (uint) game_state.turn_duration / 1000; // TODO - check
    while (game_state.turn_number <= game_state.game_length.get_num()) {
        sleep(turn_duration);
        handle_game_turn();
    }

    // GameEnded
    GameEndedMessage game_ended_message{game_state.scores};
    send_message(game_ended_message.serialize());

    game_state.is_lobby = true;
}


void ServerGameHandler::operator()() {
    try {
        while (true) {
            if(game_state.is_lobby) {
                handle_lobby();
            }
            else {
                handle_game();
            }

            // TODO -??
            lock_all_receiving_queues_in_player_clients();
            clean_all_client_queues();
            unlock_all_receiving_queues_in_player_clients();
        }
    }
    catch (std::exception &exception) {
        std::cerr << exception.what() << std::endl;
        callback_function();
    }
}
