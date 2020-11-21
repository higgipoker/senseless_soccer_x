#pragma once
#include "PlayerStateStand.hpp"
#include "PlayerStateRun.hpp"
#include "../graphics/Sprite.hpp"
#include "../input/Controller.hpp"
#include <SFML/System/Vector3.hpp>
#include <iostream>

namespace ss {
namespace game {
    
class Player {
public:
    Player();
    int id = -1;

    void update();
    void handleInput(const ControllerState& controller_state);

    sf::Vector3f position;
    sf::Vector3f velocity;
    sf::Vector3f heading;
    sf::Vector3f force;
    float speed = 0.2;
    float acceleration = 10;
    Sprite* sprite = nullptr;
    
private:
    void update_position();
    void rotate_towards(const sf::Vector3f& dir);
    sf::Vector3f target_dir;
    
    PlayerStateStand stand;
    PlayerStateRun run;
    PlayerState* current_state = &stand;
    
public:
    friend class PlayerStateStand;
    friend class PlayerStateRun;
};
}// namespace game
}// namespace ss
