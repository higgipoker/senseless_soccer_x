#pragma once
#include <SFML/System/Vector2.hpp>
namespace ss {
enum class GamepadEvent {ButtonPressed, ButtonReleased };
class Gamepad {
public:
    Gamepad();
    sf::Vector2f getLeftAnalogDirection();
private:
};
}
