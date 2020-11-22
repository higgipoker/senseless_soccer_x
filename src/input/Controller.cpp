#include "Controller.hpp"

namespace ss {
void Controller::update() {
    switch (type) {
    case ControllerType::Gamepad:
        gamepad.update (state);
        break;

    case ControllerType::Keyboard:
        kb.update (state);
        break;
    }
}

void Controller::calibrate (const Calibration& left, const Calibration& right) {
    if (type == ControllerType::Gamepad) {
        gamepad.calibrate (left, right);
    }
}

bool Controller::isCalibrated() {
    if (type == ControllerType::Gamepad) {
        return gamepad.calibrated;
    }
    return true;
}

void Controller::init() {
    if (type == ControllerType::Gamepad) {
        gamepad.setSaneDefaults();
    }
}

void Controller::unCalibrate() {
    if (type == ControllerType::Gamepad) {
        gamepad.unCalibrate();
    }
}
} // namespace ss
