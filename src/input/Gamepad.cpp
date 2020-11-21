#include "Gamepad.hpp"

#include <iostream>
#include <cmath>
namespace ss {
using namespace sf;

Gamepad::Gamepad() {
    fire_params.fire_tap_length = 6;
    fire_params.fire_double_tap_length = 8;
}

void Gamepad::update() {
    Joystick::update();
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
        if (Joystick::getAxisPosition (sf_joystick_index, Joystick::Y) <
                -thumbstick_threshold) {
            directionmask |= mask_stick_up;
        }

        if (Joystick::getAxisPosition (sf_joystick_index, Joystick::Y) >
                thumbstick_threshold) {
            directionmask |= mask_stick_down;
        }

        if (Joystick::getAxisPosition (sf_joystick_index, Joystick::X) <
                -thumbstick_threshold) {
            directionmask |= mask_stick_left;
        }

        if (Joystick::getAxisPosition (sf_joystick_index, Joystick::X) >
                thumbstick_threshold) {
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
            notify (InputEvent::SingleTap, params);
            ////////////////////////////////////////////////////////////
        }

        // fire was up and is now down
        else if ( (buttonmask & mask_a) && (old_buttonmask & mask_a) == 0) {
            fire_params.fire_ticks = 0;
            ////////////////////////////////////////////////////////////
            notify (InputEvent::FireDown, params);
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
                    notify (InputEvent::DoubleTap, params);
                    ////////////////////////////////////////////////////////////
                } else {
                    fire_params.cached_tap = true;
                }
            } else {
                ////////////////////////////////////////////////////////////
                params.push_back (fire_params.FireLengthCached);
                notify (InputEvent::FireUp, params);
                ////////////////////////////////////////////////////////////
            }
        }
        // save old states to check for events
        old_buttonmask = buttonmask;
    }
}

Vector2f Gamepad::getAxisPosition() {
    Vector2f pos =  {   Joystick::getAxisPosition (sf_joystick_index, Joystick::X), 
                        Joystick::getAxisPosition (sf_joystick_index, Joystick::Y) 
                    };
    if(fabsf(pos.x)<thumbstick_threshold) pos.x = 0;
    if(fabsf(pos.y)<thumbstick_threshold) pos.y = 0;
    return pos;
}
}  // namespace Senseless
