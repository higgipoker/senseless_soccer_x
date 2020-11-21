#pragma once
#include "controller_tools.hpp"
#include <SFML/Window/Joystick.hpp>
#include <iostream>

namespace ss {
class GamepadController {
public:
    void update (ControllerState& s);
private:
    size_t sf_joystick_index = 0;
    size_t thumbstick_threshold_x = 6;
    size_t thumbstick_threshold_y = 8;

    sf::Vector3f get_axis_vector (const sf::Joystick::Axis axis1, const sf::Joystick::Axis axis2);

    //debug
    float max_x = 100;
    float max_y = 100;

    float inner_activation_radius = 0.3f;
    float outer_activation_radius = 0.8f;

};
}
