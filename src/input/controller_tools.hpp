#pragma once
#include <SFML/System/Vector3.hpp>
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
    sf::Vector3f left_stick_vector;
    sf::Vector3f right_stick_vector;
    std::array<ButtonState, MAX_BUTTONS> buttons;
};
}
