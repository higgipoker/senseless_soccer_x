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
    void setId(const size_t id);
    bool up();
    bool down();
    bool left();
    bool right();
    bool fire();
    bool anyInput();
    void calibrate (const calibration::Calibration& cali);
    bool isCalibrated();
    void init();
    void unCalibrate();
    calibration::Calibration act_calibration;
private:
    KeyboardController kb;
    GamepadController gamepad;
public:
    friend class KeyboardController;
    friend class GamepadController;

};
}

