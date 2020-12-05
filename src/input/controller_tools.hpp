#pragma once
#include "../files/files.h"
#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>
#include <map>
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

enum class Button {
    None,
    DPadUp,
    DPadDown,
    DPadLeft,
    DPadRight,

    Button1,
    Button2,
    Button3,
    Button4,

    ShoulderLeft1,
    ShoulderLeft2,
    Shoulderright1,
    Shoulderright2,

    Start,
    Select
};
struct ControllerState {
    sf::Vector2f left_stick_vector;
    sf::Vector2f right_stick_vector;
    sf::Vector2f left_stick_raw;
    sf::Vector2f right_stick_raw;
    sf::Vector2f dpad_vector;
    std::array<ButtonState, MAX_BUTTONS> buttons;
    Button last_event_button {Button::Select};
    ButtonEvent last_event {ButtonEvent::None};
};

namespace calibration {
enum class Stick {Left, Right};
struct Calibration {
    std::string name;
    int product_id = 0;
    int vendor_id = 0;
    struct {
        sf::Vector2f min{1000.f, 1000.f};
        sf::Vector2f max{0, 0};
    } at_rest_left, at_rest_right;

    struct {
        sf::Vector2f min{1000.f, 1000.f};
        sf::Vector2f max{0, 0};
    } extremities_left, extremities_right;

    sf::Vector2f range_left, range_right;
    float activation_threshhold = 0;
    void reset() {
        at_rest_left.min = at_rest_right.min = {1000.f, 1000.f};
        at_rest_left.max = at_rest_right.max = {0.f, 0.f};
        range_left = range_right = {};
    }
};

bool load_from_file (std::map<int, Calibration>& calibrations, files::File& file);
bool write_to_file (const int id, const Calibration& calibration, files::File& file);
}
}
