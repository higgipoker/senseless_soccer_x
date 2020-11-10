#pragma once
#include "input.h"
#include <SFML/System/Vector2.hpp>
namespace ss {

class Gamepad {
public:
    Gamepad();
    void update();
    sf::Vector2f getLeftAnalogDirection();
private:

};
}
