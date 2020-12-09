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

enum class Stick {Left, Right};
struct Calibration {
    std::string name;
    unsigned int product_id = 0;
    unsigned int vendor_id = 0;
    bool calibrating = false;

    struct {
        std::vector<sf::Vector2f> samples;
        sf::Vector2f at_rest_before;
        sf::Vector2f at_rest_after;
        sf::Vector2f inner_dead_zone;
        sf::Vector2f outer_max;
        sf::Vector2f activation_threshhold;

    } left_stick, right_stick;
};
void on_calibration_started (Calibration* cali);
void on_calibration_finished (Calibration* cali, int id);
void on_calibration_cancelled (Calibration* cali);
bool load_from_file (std::map<int, Calibration>& calibrations, files::File& file);
bool write_to_file (const int id, const Calibration& calibration, files::File& file);
}
