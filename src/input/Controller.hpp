#pragma once
#include "controller_tools.hpp"
#include "KeyboardController.hpp"
#include "GamepadController.hpp"

namespace ss {

enum class ControllerType {Keyboard, Gamepad};

class Controller {
public:
    void update();
    ControllerType type = ControllerType::Gamepad;
    ControllerState state;
    bool up();
    bool down();
    bool left();
    bool right();
    void calibrate(const Calibration& left, const Calibration &right);
    bool isCalibrated();
    void init();
    void unCalibrate();
private:
    KeyboardController kb;
    GamepadController gamepad;
public:
    friend class KeyboardController;
    friend class GamepadController;

};
}

