#include "GamepadController.hpp"
#include "../math/vector.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>

namespace ss {
using namespace sf;

void GamepadController::update (ControllerState& s) {
    Joystick::update();
    // analog stick direction TODO separate tolerances for left and irght stick
    s.left_stick_vector =  get_axis_vector (sf::Joystick::X, sf::Joystick::Y);
    s.right_stick_vector = get_axis_vector (sf::Joystick::U, sf::Joystick::V);


    // if stick is centered, check the dpad
    if (s.left_stick_vector.x == 0 && s.left_stick_vector.y == 0) {
        if (Joystick::getAxisPosition (sf_joystick_index, Joystick::Axis::PovX) < 0) {
            s.left_stick_vector.x = -1;
        }
        if (Joystick::getAxisPosition (sf_joystick_index, Joystick::Axis::PovX) > 0) {
            s.left_stick_vector.x = 1;
        }
        if (Joystick::getAxisPosition (sf_joystick_index, Joystick::Axis::PovY) < 0) {
            s.left_stick_vector.y = -1;
        }
        if (Joystick::getAxisPosition (sf_joystick_index, Joystick::Axis::PovY) > 0) {
            s.left_stick_vector.y = 1;
        }

        s.left_stick_vector = vec_normalized2d (s.left_stick_vector);
    }
}

sf::Vector3f GamepadController::get_axis_vector (const sf::Joystick::Axis axis1, const sf::Joystick::Axis axis2) {
    sf::Vector3f ret =  {   Joystick::getAxisPosition (sf_joystick_index, axis1),
                            Joystick::getAxisPosition (sf_joystick_index, axis2),
                            0
                        };
    max_x = std::max (max_x, ret.x);
    max_y = std::max (max_y, ret.y);

    if (fabsf (ret.x) < thumbstick_threshold_x) ret.x = 0;
    if (fabsf (ret.y) < thumbstick_threshold_y) ret.y = 0;

    // normalize to values between -1 and 1
    ret.x /= max_x;
    ret.y /= max_y;

    if (greater_than (ret.x, 1.f)) ret.x = 1.f;
    if (greater_than (ret.y, 1.f)) ret.y = 1.f;
    
    ret.x = std::clamp(ret.x, -1.f, 1.f);
    ret.y = std::clamp(ret.y, -1.f, 1.f);

    if (less_than (vec_magnitude2d (ret), inner_activation_radius)) {
        vec_reset (ret);
    }

    return ret;
}
}
