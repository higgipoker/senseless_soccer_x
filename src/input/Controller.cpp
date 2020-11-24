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

bool Controller::up() {
    bool ret = false;
    switch (type) {
    case ControllerType::Gamepad:
        return gamepad.up();
        break;
    case ControllerType::Keyboard:
        break;
    }
    return ret;
}

bool Controller::down() {
    bool ret = false;
    switch (type) {
    case ControllerType::Gamepad:
        return gamepad.down();
        break;
    case ControllerType::Keyboard:
        break;
    }
    return ret;

}

bool Controller::left() {
    bool ret = false;
    switch (type) {
    case ControllerType::Gamepad:
        return gamepad.left();
        break;
    case ControllerType::Keyboard:
        break;
    }
    return ret;
}

bool Controller::right() {
    bool ret = false;
    switch (type) {
    case ControllerType::Gamepad:
        return gamepad.right();
        break;
    case ControllerType::Keyboard:
        break;
    }
    return ret;
}
} // namespace ss
