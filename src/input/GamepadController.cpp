#include "GamepadController.hpp"
#include "../math/vector.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>

namespace ss {
using namespace sf;

GamepadController::GamepadController() {
    setSaneDefaults();
}

void GamepadController::update (ControllerState& s) {
    Joystick::update();
    // analog stick direction TODO separate tolerances for left and irght stick
    s.left_stick_vector =  get_axis_vector (sf::Joystick::X, sf::Joystick::Y, left_stick_calibration);
    s.right_stick_vector = get_axis_vector (sf::Joystick::U, sf::Joystick::V, right_stick_calibration);

    s.dpad_vector = get_dpad_vector();

    //tmp
    if (Joystick::isButtonPressed (sf_joystick_index, 0)) {
        s.buttons[0].evt = ButtonEvent::Pressed;
    } else {
        s.buttons[0].evt = ButtonEvent::None;
    }

    /*
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
        */
}

sf::Vector3f GamepadController::get_axis_vector (const sf::Joystick::Axis axis1, const sf::Joystick::Axis axis2, const Calibration& calibration) {
    sf::Vector3f ret =  {   Joystick::getAxisPosition (sf_joystick_index, axis1),
                            Joystick::getAxisPosition (sf_joystick_index, axis2),
                            0
                        };

    if (calibrated) {
        // raw vals are lower than calibreated dead zone -> set them to zero
        if (fabsf (ret.x) < calibration.at_rest.min.x) ret.x = 0;
        if (fabsf (ret.y) < calibration.at_rest.min.y) ret.y = 0;

        // normalize to values between -1 and 1
        ret.x /= calibration.range.x;
        ret.y /= calibration.range.y;


        ret.x = std::clamp (ret.x, -1.f, 1.f);
        ret.y = std::clamp (ret.y, -1.f, 1.f);

        // TODO
//     if (less_than (vec_magnitude2d (ret), inner_activation_radius)) {
//         vec_reset (ret);
//     }
    }

    return ret;
}

sf::Vector2f GamepadController::get_dpad_vector() {
    sf::Vector2f dpad =
    {sf::Joystick::getAxisPosition (sf_joystick_index, sf::Joystick::PovX),  sf::Joystick::getAxisPosition (sf_joystick_index, sf::Joystick::PovY) };
    return dpad;
}

void GamepadController::setSaneDefaults() {
    calibrated = true;
    left_stick_calibration.at_rest.min = {10, 10};
    left_stick_calibration.extremities.min = {0, 0};
    left_stick_calibration.extremities.max = {100, 100};
    left_stick_calibration.range = {100, 100};

    right_stick_calibration = left_stick_calibration;
}

void GamepadController::calibrate (const Calibration& left, const Calibration& right) {
    calibrated = true;
    left_stick_calibration = left;
    right_stick_calibration = right;

    left_stick_calibration.range.x = left_stick_calibration.extremities.max.x - left_stick_calibration.at_rest.min.x;
    left_stick_calibration.range.y = left_stick_calibration.extremities.max.y - left_stick_calibration.at_rest.min.y;

    right_stick_calibration.range.x = right_stick_calibration.extremities.max.x - right_stick_calibration.at_rest.min.x;
    right_stick_calibration.range.y = right_stick_calibration.extremities.max.y - right_stick_calibration.at_rest.min.y;
}

void GamepadController::unCalibrate() {
    calibrated = false;
    left_stick_calibration.reset();
    right_stick_calibration.reset();
}
}
