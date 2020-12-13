#include "GamepadController.hpp"
#include "../math/vector.hpp"
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>

namespace ss {
using namespace sf;

GamepadController::GamepadController() {
}

void GamepadController::update (ControllerState& s) {
    // analog stick direction
    std::pair<sf::Vector2f, sf::Vector2f> data = get_axis_vector (sf::Joystick::X, sf::Joystick::Y, Stick::Left);
    s.left_stick_raw = data.first;
    s.left_stick_vector =  data.second;

    data = get_axis_vector (sf::Joystick::U, sf::Joystick::V, Stick::Right);
    s.right_stick_raw = data.first;
    s.right_stick_vector = data.second;

    s.dpad_vector = get_dpad_vector();

    directionmask = mask_zero;
    buttonmask = mask_zero;

    s.last_event = ButtonEvent::None;

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
        if (s.left_stick_vector.y < -calibration.activation_threshhold) {
            directionmask |= mask_stick_up;
        }

        if (s.left_stick_vector.y > calibration.activation_threshhold) {
            directionmask |= mask_stick_down;
        }

        if (s.left_stick_vector.x < -calibration.activation_threshhold) {
            directionmask |= mask_stick_left;
        }

        if (s.left_stick_vector.x > calibration.activation_threshhold) {
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
            s.last_event = ButtonEvent::Pressed;
            s.last_event_button = Button::Button1;
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

std::pair<sf::Vector2f, sf::Vector2f> GamepadController::get_axis_vector (const sf::Joystick::Axis axis1, const sf::Joystick::Axis axis2, const  Stick stick) {
    std::pair<sf::Vector2f, sf::Vector2f> ret;
    ret.first = ret.second =  {   Joystick::getAxisPosition (sf_joystick_index, axis1),
                                  Joystick::getAxisPosition (sf_joystick_index, axis2),
                              };

    // raw vals are lower than calibreated dead zone -> set them to zero
    sf::Vector2f dead_zone = stick ==  Stick::Left ? calibration.left_stick.inner_dead_zone : calibration.right_stick.inner_dead_zone;
    if (less_than_or_equal (vec_magnitude (ret.second), vec_magnitude (dead_zone))) {
        vec_reset (ret.second);
    }

//     dead_zone = stick ==  Stick::Left ? calibration.left_stick.outer_dead_zone : calibration.right_stick.outer_dead_zone;
//     if (fabsf (ret.second.x) > dead_zone.x)
//         ret.second.x = dead_zone.x;
//     if (fabsf (ret.second.y) > dead_zone.y)
//         ret.second.y = dead_zone.y;

    // normalize to values between -1 and 1
    sf::Vector2f range = stick ==  Stick::Left ? calibration.left_stick.range : calibration.right_stick.range;
    ret.second.x = std::min (ret.second.x, range.x);
    ret.second.y = std::min (ret.second.y, range.y);
    ret.second.x /= range.x;
    ret.second.y /= range.y;

    ret.second.x = std::clamp (ret.second.x, -1.f, 1.f);
    ret.second.y = std::clamp (ret.second.y, -1.f, 1.f);

    return ret;
}

sf::Vector2f GamepadController::get_dpad_vector() {
    sf::Vector2f dpad =
    {sf::Joystick::getAxisPosition (sf_joystick_index, sf::Joystick::PovX),  sf::Joystick::getAxisPosition (sf_joystick_index, sf::Joystick::PovY) };
    return dpad;
}

void GamepadController::calibrate (Calibration* cali) {
    calibrated = true;

    calibration.name = cali->name;
    calibration.vendor_id = cali->vendor_id;
    calibration.product_id = cali->product_id;

    calibration.left_stick.inner_dead_zone = {cali->left_stick.inner_dead_zone};
    calibration.left_stick.outer_dead_zone = {cali->left_stick.outer_max * 0.9f}; // TODO calc proper outer dead zone
    calibration.left_stick.range = {fabsf (calibration.left_stick.outer_dead_zone.x) - fabsf (calibration.left_stick.inner_dead_zone.x), fabsf (cali->left_stick.outer_max.y) - fabsf (cali->left_stick.inner_dead_zone.y) };


    calibration.right_stick.inner_dead_zone = {cali->right_stick.inner_dead_zone};
    calibration.right_stick.outer_dead_zone = {cali->right_stick.outer_max * 0.9f}; // TODO calc proper outer dead zone
    calibration.right_stick.range = {fabsf (cali->right_stick.outer_max.x) - fabsf (cali->right_stick.inner_dead_zone.x), fabsf (cali->right_stick.outer_max.y) - fabsf (cali->right_stick.inner_dead_zone.y) };

}

void GamepadController::unCalibrate() {
    calibrated = false;
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

bool GamepadController::fire() {
    return ( (buttonmask & mask_a) || (buttonmask & mask_b) || (buttonmask & mask_x) || (buttonmask & mask_y) || (buttonmask & mask_start));
}
}


