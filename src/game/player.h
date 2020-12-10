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


enum State {Standing, Running, Strafing };

struct Player {
    State       state           = State::Standing;
    int         id              = -1;

    struct {
        float       speed           = 10;
        float       acceleration    = 0.1f;
    } attributes;
};

void update(Player* player, engine::MatchEngine* engine);
void handle_input (Player* player, const ControllerState& controller);
}// namespace player
}// namespace ss
