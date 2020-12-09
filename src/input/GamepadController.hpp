#pragma once
#include "controller_tools.hpp"
#include <SFML/Window/Joystick.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

namespace ss {

const unsigned char mask_zero{0};        // 0000 0000
const unsigned char mask_a{1 << 0};      // 0000 0001
const unsigned char mask_b{1 << 1};      // 0000 0010
const unsigned char mask_x{1 << 2};      // 0000 0100
const unsigned char mask_y{1 << 3};      // 0000 1000
const unsigned char mask_l1{1 << 4};     // 0001 0000
const unsigned char mask_r1{1 << 5};     // 0010 0000
const unsigned char mask_back{1 << 6};   // 0100 0000
const unsigned char mask_start{1 << 7};  // 1000 0000

const unsigned char mask_dpad_up{1 << 0};      // 0000 0001
const unsigned char mask_dpad_right{1 << 1};   // 0000 0010
const unsigned char mask_dpad_down{1 << 2};    // 0000 0100
const unsigned char mask_dpad_left{1 << 3};    // 0000 1000
const unsigned char mask_stick_up{1 << 4};     // 0001 0000
const unsigned char mask_stick_right{1 << 5};  // 0010 0000
const unsigned char mask_stick_down{1 << 6};   // 0100 0000
const unsigned char mask_stick_left{1 << 7};   // 1000 0000


class GamepadController {
public:
    GamepadController();
    void update (ControllerState& s);
    void calibrate (Calibration* cali);
    void unCalibrate();
    bool up();
    bool down();
    bool left();
    bool right();
    bool fire();

    size_t sf_joystick_index = 0;
    size_t thumbstick_threshold_x = 0;
    size_t thumbstick_threshold_y = 0;

    bool calibrated = false;
    bool thumbsticks_enabled = true;
    bool dpad_enabled = true;

private:

    struct {
        int FireLength          = 0;
        int FireLengthCached    = 0;
        int fire_ticks          = 0;
        int ticks_since_tap     = 0;
        bool cached_tap         = false;

        int fire_tap_length = 4;
        int fire_double_tap_length = 12;
    } fire_params;

    struct {
        std::string name;
        int vendor_id {-1};
        int product_id{-1};
        struct {
            sf::Vector2f inner_dead_zone    {10, 10};
            sf::Vector2f outer_dead_zone    {90, 90};
            sf::Vector2f range              {80, 80};
        } left_stick, right_stick;
        
        // tmp when analog sticks register as a direction in menus
        float activation_threshhold = 0.4f;
    } calibration;
    void reset_calibration(){
        calibration.left_stick.inner_dead_zone = {10,10};
        calibration.left_stick.outer_dead_zone = {90,90};
        calibration.left_stick.range = {80,80};
        calibration.activation_threshhold = 0.4f;
    }

    unsigned char buttonmask{0x0};
    unsigned char directionmask{0x0};
    unsigned char old_buttonmask{0x0};

    std::pair<sf::Vector2f, sf::Vector2f> get_axis_vector (const sf::Joystick::Axis axis1, const sf::Joystick::Axis axis2, const  Stick stick);
    sf::Vector2f get_dpad_vector();
    float inner_activation_radius = 0.3f;
    float outer_activation_radius = 0.8f;

};
}
