#pragma once
#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>

namespace ss {
constexpr size_t MAX_BUTTONS = 8;
enum class ButtonEvent {
    None,
    Pressed,
    Released,
    Tapped,
    DoubleTapped
};

struct ButtonState {
    ButtonEvent evt;
    int time;
};

struct ControllerState {
    sf::Vector2f left_stick_vector;
    sf::Vector2f right_stick_vector;
    sf::Vector2f left_stick_raw;
    sf::Vector2f right_stick_raw;
    sf::Vector2f dpad_vector;
    std::array<ButtonState, MAX_BUTTONS> buttons;
};

struct Calibration {
    struct {
        sf::Vector2f min{1000.f, 1000.f};
        sf::Vector2f max{0, 0};
    } at_rest;

    struct {
        sf::Vector2f min{1000.f, 1000.f};
        sf::Vector2f max{0, 0};
    } extremities;

    sf::Vector2f range;
    float activation_threshhold = 0;
    void reset() {
        at_rest.min.x = at_rest.min.y = 1000.f;
        at_rest.max.x = at_rest.max.y = 0.f;

        extremities.min.x = extremities.min.y = 1000.f;
        extremities.max.x = extremities.max.y = 0.f;

        range.x = range.y = 0;
    }
};
}
