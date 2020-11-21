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
private:
    KeyboardController kb;
    GamepadController gamepad;
public:
    friend class KeyboardController;
    friend class GamepadController;

};
}
