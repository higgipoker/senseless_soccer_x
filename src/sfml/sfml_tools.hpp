#pragma once
#include <SFML/Window/Joystick.hpp>
#include <SFML/Graphics/Color.hpp>
#include <array>

namespace sfml {

//
// Joystick Tools
//
static inline std::array<bool, sf::Joystick::Count> connected_joysticks() {
    std::array<bool, sf::Joystick::Count> joysticks;
    for (size_t i = 0; i < joysticks.size(); ++i) {
        if (sf::Joystick::isConnected (i)) {
            joysticks[i] = true;
        } else {
            joysticks[i] = false;
        }
    }
    return joysticks;
}

//
// Colors
//
static inline const sf::Color DarkGreen = sf::Color(0, 60, 0);

}
