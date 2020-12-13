#pragma once

#include "entity.h"
#include "../input/controller_tools.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <iostream>

namespace ss {

namespace engine {
struct MatchEngine;
}

namespace player {
// ********************
// Attributes
// ********************
struct Attributes {
    float speed         = 10.f;
    float acceleration  = 1.f;
};
// ********************
// Player
// ********************
struct Player {
    enum State {Standing, Running, Jumping, Sliding, Strafing } state = Standing; 
    int id = -1;
    sf::CircleShape feet;
};

void init               (Player* player, engine::MatchEngine* engine);
void update             (Player* player, engine::MatchEngine* engine);
void handle_input       (Player* player, engine::MatchEngine* engine, const ControllerState& controller);
void change_state       (Player* player, engine::MatchEngine* engine, const Player::State state);
void on_state_started   (Player* player, engine::MatchEngine* engine);
void on_state_ended     (Player* player, engine::MatchEngine* engine);
bool state_over         (Player* player, engine::MatchEngine* engine);
}// namespace player
}// namespace ss
