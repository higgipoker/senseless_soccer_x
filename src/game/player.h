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



struct Player {
    enum State {Standing, Running, Strafing } state = Standing; 
    int id = -1;
};
void init(Player* player, engine::MatchEngine* engine);
void update (Player* player, engine::MatchEngine* engine);
void handle_input (Player* player, const ControllerState& controller, engine::MatchEngine* engine);
void change_state(Player* player, const Player::State state, engine::MatchEngine* engine);
void on_state_started(Player* player, engine::MatchEngine* engine);
void on_state_ended(Player* player, engine::MatchEngine* engine);
bool state_over(Player*player, engine::MatchEngine* engine);
}// namespace player
}// namespace ss
