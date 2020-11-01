#include "gamepad.h"
#include <SFML/Window/Joystick.hpp>
namespace ss {

Gamepad::Gamepad() {

}
sf::Vector2f Gamepad::getLeftAnalogDirection() {
    return { sf::Joystick::getAxisPosition(0, sf::Joystick::X), sf::Joystick::getAxisPosition(0, sf::Joystick::Y)};
}

}
