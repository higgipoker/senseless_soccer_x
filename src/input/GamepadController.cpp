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

    // analog stick direction
    s.left_stick_vector =  get_axis_vector (sf::Joystick::X, sf::Joystick::Y, left_stick_calibration);
    s.right_stick_vector = get_axis_vector (sf::Joystick::U, sf::Joystick::V, right_stick_calibration);
    s.dpad_vector = get_dpad_vector();

    directionmask = mask_zero;
    buttonmask = mask_zero;

    {
        // set button mask
        if (Joystick::isButtonPressed (sf_joystick_index, 0)) {
            buttonmask |= mask_a;
        }

        if (Joystick::isButtonPressed (sf_joystick_index, 1)) {
            buttonmask |= mask_b;
        }

        if (Joystick::isButtonPressed (sf_joystick_index, 2)) {
            buttonmask |= mask_x;
        }

        if (Joystick::isButtonPressed (sf_joystick_index, 3)) {
            buttonmask |= mask_y;
        }

        if (Joystick::isButtonPressed (sf_joystick_index, 4)) {
            buttonmask |= mask_l1;
        }

        if (Joystick::isButtonPressed (sf_joystick_index, 5)) {
            buttonmask |= mask_r1;
        }

        if (Joystick::isButtonPressed (sf_joystick_index, 6)) {
            buttonmask |= mask_back;
        }

        if (Joystick::isButtonPressed (sf_joystick_index, 7)) {
            buttonmask |= mask_start;
        }
    }
    {
        // set dpad mask
        if (Joystick::getAxisPosition (sf_joystick_index, Joystick::Axis::PovX) <
                0) {
            directionmask |= mask_dpad_left;
        }
        if (Joystick::getAxisPosition (sf_joystick_index, Joystick::Axis::PovX) >
                0) {
            directionmask |= mask_dpad_right;
        }
        if (Joystick::getAxisPosition (sf_joystick_index, Joystick::Axis::PovY) <
                0) {
            directionmask |= mask_dpad_up;
        }
        if (Joystick::getAxisPosition (sf_joystick_index, Joystick::Axis::PovY) >
                0) {
            directionmask |= mask_dpad_down;
        }
    }
    {
        // set stick mask
        if (s.left_stick_vector.y <= -left_stick_calibration.activation_threshhold) {
            directionmask |= mask_stick_up;
        }

        if (s.left_stick_vector.y >= left_stick_calibration.activation_threshhold) {
            directionmask |= mask_stick_down;
        }

        if (s.left_stick_vector.x <= -left_stick_calibration.activation_threshhold) {
            directionmask |= mask_stick_left;
        }

        if (s.left_stick_vector.x >= left_stick_calibration.activation_threshhold) {
            directionmask |= mask_stick_right;
        }
    }
    {
        // detect events
        std::vector<int> params;
        // if there was a cached tap, see if enough time has elapsed to make it a
        // tap now and not a possible double tap  if (fire_params.cached_tap) {
        if (fire_params.cached_tap &&
                ++fire_params.ticks_since_tap > fire_params.fire_double_tap_length) {
            // time up -> this is a tap and not a double tap
            fire_params.cached_tap = false;
            fire_params.ticks_since_tap = 0;
            ////////////////////////////////////////////////////////////
            // TODO set event flag here for TAP
            ////////////////////////////////////////////////////////////
        }

        // fire was up and is now down
        else if ( (buttonmask & mask_a) && (old_buttonmask & mask_a) == 0) {
            fire_params.fire_ticks = 0;
            ////////////////////////////////////////////////////////////
            // TODO set event flag here for firedown
            ////////////////////////////////////////////////////////////
        }

        // fire is still down
        else if (buttonmask & mask_a && (old_buttonmask & mask_a)) {
            fire_params.FireLength = fire_params.fire_ticks++;
        }

        // fire was down and is now up
        else if ( (buttonmask & mask_a) == 0 && old_buttonmask & mask_a) {
            fire_params.FireLengthCached = fire_params.fire_ticks;
            fire_params.FireLength = 0;
            if (fire_params.fire_ticks < fire_params.fire_tap_length) {
                // was there a tap cached?
                if (fire_params.cached_tap) {
                    // so this is a double tap
                    fire_params.cached_tap = false;
                    fire_params.ticks_since_tap = 0;
                    ////////////////////////////////////////////////////////////
                    // TODO set event flag here for doubletap
                    ////////////////////////////////////////////////////////////
                } else {
                    fire_params.cached_tap = true;
                }
            } else {
                ////////////////////////////////////////////////////////////
                params.push_back (fire_params.FireLengthCached);
                // TODO set event flag here for fire up
                ////////////////////////////////////////////////////////////
            }
        }
        // save old states to check for events
        old_buttonmask = buttonmask;
    }



    //tmp
    if (Joystick::isButtonPressed (sf_joystick_index, 0)) {
        s.buttons[0].evt = ButtonEvent::Pressed;
    } else {
        s.buttons[0].evt = ButtonEvent::None;
    }
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
    left_stick_calibration.activation_threshhold = 0.5f;

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

bool GamepadController::up() {
    if (dpad_enabled && directionmask & mask_dpad_up) {
        return true;
    }
    if (thumbsticks_enabled && directionmask & mask_stick_up) {
        return true;
    }
    return false;
}

bool GamepadController::down() {
    if (dpad_enabled && directionmask & mask_dpad_down) {
        return true;
    }
    if (thumbsticks_enabled && directionmask & mask_stick_down) {
        return true;
    }
    return false;
}

bool GamepadController::left() {
    if (dpad_enabled && directionmask & mask_dpad_left) {
        return true;
    }
    if (thumbsticks_enabled && directionmask & mask_stick_left) {
        return true;
    }
    return false;
}

bool GamepadController::right() {
    if (dpad_enabled && directionmask & mask_dpad_right) {
        return true;
    }
    if (thumbsticks_enabled && directionmask & mask_stick_right) {
        return true;
    }
    return false;
}
}
