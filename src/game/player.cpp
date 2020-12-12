#include "player.h"
#include "engine.h"
#include "../math/math.hpp"
#include "../math/vector.hpp"
#include <iostream>

namespace ss {
namespace player {

using namespace engine;

void init (Player* player) {
    player->movable.type = engine::Movable::Two_D;
}

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

    // update sprite
    auto sprite = engine::sprite (engine, player->id);
    sprite->sprite.setPosition (player->movable.movable2.position);
    sprite->sprite.setRotation (vec_angle (player->movable.movable2.velocity) - 90);
}

void handle_input (Player* player, const ControllerState& controller) {

    if (vec_magnitude (controller.left_stick_vector)) {
        std::cout << controller.left_stick_vector.y << std::endl;
    }
}

}// namespace player
}// namespace ss
