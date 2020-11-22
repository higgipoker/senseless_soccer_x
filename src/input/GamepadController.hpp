#pragma once
#include "controller_tools.hpp"
#include <SFML/Window/Joystick.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

namespace ss {



class GamepadController {
public:
    GamepadController();
    void update (ControllerState& s);
    void setSaneDefaults();
    void calibrate (const Calibration& left, const Calibration& right);
    void unCalibrate();
    Calibration left_stick_calibration;
    Calibration right_stick_calibration;

    size_t thumbstick_threshold_x = 0;
    size_t thumbstick_threshold_y = 0;

    bool calibrated = false;

private:
    size_t sf_joystick_index = 0;
    sf::Vector3f get_axis_vector (const sf::Joystick::Axis axis1, const sf::Joystick::Axis axis2, const Calibration& calibration);
    
    sf::Vector2f get_dpad_vector();

    float inner_activation_radius = 0.3f;
    float outer_activation_radius = 0.8f;



};
}
