#pragma once
#include "Page.hpp"
#include "GamepadWidget.hpp"
#include "../input/Controller.hpp"
#include "ListBox.hpp"
#include <SFML/Window/Joystick.hpp>
#include <array>
#include <limits>

namespace ss {

enum class TestPhase {Idle, At_Rest, Extremities};

class PageCalibrate: public Page {
public:
    PageCalibrate (sf::RenderWindow& wnd, Menu* ctx);
    GamepadController* gamepad;
    void onShow() override;
    void onHide() override;
    void update() override;
    void handleButtonPress (Widget* button) override;
private:
    GamepadWidget* gamepad_widget = nullptr;
    void draw_self() override;
    std::array<bool, sf::Joystick::Count> joysticks;

    TestPhase phase = TestPhase::Idle;
    int count_ticks = 0;
    int calibration_ticks = 60 * 2;
    bool read_dead_zone = true;
    bool phase_done = false;

    Calibration calibration;
    std::vector<Widget*> listbox_rows;
};

}
