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
    change_state (player, engine, Player::Standing);
    
    player->feet.setRadius(5);
    player->feet.setOrigin(5,5);
    player->feet.setFillColor({0,0,0,0});
    player->feet.setOutlineColor(sf::Color::Red);
    player->feet.setOutlineThickness(1);
}

void change_state (Player* player, engine::MatchEngine* engine, const Player::State state) {
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
    case Player::Jumping:
        break;
    case Player::Sliding:
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
    case Player::Jumping:
        break;
    case Player::Sliding:
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
    case Player::Jumping:
        break;
    case Player::Sliding:
        break;
    case Player::Strafing:
        break;
    }

    return false;
}

void update (Player* player, MatchEngine* engine) {
    auto movable = &engine->sprites.movable[player->id].movable2;
    auto sprite  = &engine->sprites.drawable[player->id].sprite;

    // state machine
    switch (player->state) {
    case Player::Standing:
        if (vec_magnitude (movable->velocity)) {
            change_state (player, engine, Player::Running);
        }
        break;
    case Player::Running:
        if (!vec_magnitude (movable->velocity)) {
            change_state (player, engine, Player::Standing);
        }
        break;
    case Player::Jumping:
        break;
    case Player::Sliding:
        break;
    case Player::Strafing:
        break;
    }

    // update sprite
    sprite->setPosition (movable->position);
    sprite->setRotation (vec_angle ({movable->heading.x, -movable->heading.y}) - 90);
    
    // update feet
    player->feet.setPosition(movable->position);
}

void handle_input (Player* player, engine::MatchEngine* engine, const ControllerState& controller) {
    auto force = engine->controllers[engine->sprites.controllable[player->id].input].state.left_stick_vector;
    engine->sprites.movable[player->id].movable2.applied_force = force;
}

}// namespace player
}// namespace ss
