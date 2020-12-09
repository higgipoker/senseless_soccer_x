#include "player.h"
#include "engine.h"
#include "../math/math.hpp"
#include "../math/vector.hpp"
#include <iostream>

namespace ss {
namespace player {

using namespace engine;

void simulate (Player* player, MatchEngine* engine, const float dt) {

    // state machine
    switch (player->state) {
    case State::Standing:
        break;
    case State::Running:
        break;
    case State::Strafing:
        break;
    }

    // physics sim
    player->acceleration = player->acceleration * player->attributes.acceleration; // acceleration is applied by controller input
    player->velocity += player->acceleration;                                       // TODO limit to player top speed attribute
    player->position = player->position + player->velocity;

    // damp very low velocities
    static const float DAMP_VELOCITY = 0.1f;
    if (less_than (vec_magnitude (player->velocity), DAMP_VELOCITY)) {
        vec_reset (player->velocity);
    }
    // reset acceleration for next frames input
    vec_reset (player->acceleration);

    // update sprite
    engine->sprite_pool[player->id].sprite.setPosition({player->position.x, player->position.y});
    engine->sprite_pool[player->id].sprite.setRotation (vec_angle (player->velocity) - 90);
}

void handle_input (Player* player, const ControllerState& controller) {
}

void rotate_towards (Player* player, const sf::Vector2f& dir) {
    player->target_heading = dir;
}

}// namespace player
}// namespace ss
