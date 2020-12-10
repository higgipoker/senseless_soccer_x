#include "player.h"
#include "engine.h"
#include "../math/math.hpp"
#include "../math/vector.hpp"
#include <iostream>

namespace ss {
namespace player {

using namespace engine;

void update (Player* player, MatchEngine* engine) {

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
    engine::sprite(engine, player->id)->sprite.setPosition({engine::player(engine, player->id)->position.x, engine::player(engine, player->id)->position.y});
    engine::sprite(engine, player->id)->sprite.setRotation (vec_angle (engine::player(engine, player->id)->velocity) - 90);
}

void handle_input (Player* player, const ControllerState& controller) {
}

}// namespace player
}// namespace ss
