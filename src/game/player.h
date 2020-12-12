#pragma once

#include "entity.h"
#include "../input/controller_tools.hpp"
#include <SFML/System/Vector2.hpp>
#include <iostream>

namespace ss {

namespace engine {
struct MatchEngine;
}

namespace player {

struct Attributes {
    float speed = 10.f;
    float acceleration = 1.f;
};

enum State {Standing, Running, Strafing };

struct Player {
    State state = State::Standing;
    engine::Movable movable;
    int  id     = -1;
};
void init(Player* player);
void simulate (Player* player, engine::MatchEngine* engine, const float dt);
void handle_input (Player* player, const ControllerState& controller);
}// namespace player
}// namespace ss
