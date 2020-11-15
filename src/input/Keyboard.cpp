#include "Keyboard.hpp"

namespace ss {

void Keyboard::update() {
    directionmask = mask_zero;
    buttonmask = mask_zero;
    if (sf::Keyboard::isKeyPressed (keymap[Buttons::DPadLeft])) {
        directionmask |= mask_dpad_left;
    }
    if (sf::Keyboard::isKeyPressed (keymap[Buttons::DPadRight])) {
        directionmask |= mask_dpad_right;
    }
    if (sf::Keyboard::isKeyPressed (keymap[Buttons::DPadUp])) {
        directionmask |= mask_dpad_up;
    }
    if (sf::Keyboard::isKeyPressed (keymap[Buttons::DPadDown])) {
        directionmask |= mask_dpad_down;
    }
    if (sf::Keyboard::isKeyPressed (keymap[Buttons::Button1])) {
        buttonmask |= mask_a;
    }
}
}  // namespace Senseless
