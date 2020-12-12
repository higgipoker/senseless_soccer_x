#pragma once
#include "engine.h"
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
    int  id     = -1;
};

void update (Player* player, engine::MatchEngine* engine);
void handle_input (Player* player, const ControllerState& controller);
}// namespace player
}// namespace ss
