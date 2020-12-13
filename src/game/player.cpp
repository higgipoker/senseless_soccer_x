#include "player.h"
#include "engine.h"
#include "../graphics/sprite_definitions.h"
#include "../math/math.hpp"
#include "../math/vector.hpp"
#include <iostream>

namespace ss {
namespace player {

using namespace engine;

void init (Player* player, MatchEngine* engine) {
    change_state (player, Player::Standing, engine);
}

void change_state (Player* player, const Player::State state, engine::MatchEngine* engine) {
    on_state_ended (player, engine);
    player->state = state;
    on_state_started (player, engine);
}

void on_state_started (Player* player, engine::MatchEngine* engine) {
    switch (player->state) {
    case Player::Standing:
        graphics::make_stand_animation (&engine->sprites.animation[player->id]);
        break;
    case Player::Running:
        graphics::make_run_animation (&engine->sprites.animation[player->id]);
        break;
    case Player::Strafing:
        break;
    }
}

void on_state_ended (Player* player, engine::MatchEngine* engine) {
    switch (player->state) {
    case Player::Standing:
        break;
    case Player::Running:
        break;
    case Player::Strafing:
        break;
    }
}

bool state_over (Player* player, engine::MatchEngine* engine) {

    switch (player->state) {
    case Player::Standing:
        break;
    case Player::Running:
        break;
    case Player::Strafing:
        break;
    }

    return false;
}

void update (Player* player, MatchEngine* engine) {

    auto velocity = engine->sprites.movable[player->id].movable2.velocity;

    // state machine
    switch (player->state) {
    case Player::Standing:
        if (vec_magnitude (velocity)) {
            change_state (player, Player::Running, engine);
        }
        break;
    case Player::Running:
        if (!vec_magnitude (velocity)) {
            change_state (player, Player::Standing, engine);
        }
        break;
    case Player::Strafing:
        break;
    }



    // update sprite
    auto sprite = &engine->sprites.drawable [player->id].sprite;
    sprite->setPosition (engine->sprites.movable[player->id].movable2.position);
    sprite->setRotation (vec_angle ({engine->sprites.movable[player->id].movable2.velocity.x, -engine->sprites.movable[player->id].movable2.velocity.y}) - 90);
}

void handle_input (Player* player, const ControllerState& controller, engine::MatchEngine* engine) {
    auto force = engine->controllers[engine->sprites.controllable[player->id].input].state.left_stick_vector;
    engine->sprites.movable[player->id].movable2.applied_force = force;
}


}// namespace player
}// namespace ss
