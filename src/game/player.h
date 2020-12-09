#pragma once
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

    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    sf::Vector2f heading;
    sf::Vector2f target_heading;
};

static void rotate_towards (Player* player, const sf::Vector2f& dir);

void simulate (Player* player, engine::MatchEngine* engine, const float dt);
void handle_input (Player* player, const ControllerState& controller);
}// namespace player
}// namespace ss
