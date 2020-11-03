#include "gamepad.h"
#include <SFML/Window/Joystick.hpp>
namespace ss {

Gamepad::Gamepad() {

}

void Gamepad::update() {
    if (sf::Joystick::isButtonPressed (0, 0)) {
    }
}
sf::Vector2f Gamepad::getLeftAnalogDirection() {
    return { sf::Joystick::getAxisPosition (0, sf::Joystick::X), sf::Joystick::getAxisPosition (0, sf::Joystick::Y) };
}

}
