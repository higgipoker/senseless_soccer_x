#pragma once
#include "../graphics/Sprite.hpp"
#include "../input/InputDevice.hpp"
#include <SFML/System/Vector3.hpp>
#include <iostream>

namespace ss {
namespace game {
    
class Player {
public:
    int id = -1;

    void update();

    sf::Vector3f position;
    sf::Vector3f velocity;
    sf::Vector3f heading;
    Sprite* sprite = nullptr;
    InputDevice* controller = nullptr;
};
}// namespace game
}// namespace ss
