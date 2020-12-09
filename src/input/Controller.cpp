#include "Controller.hpp"
#include "../math/vector.hpp"

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

void Controller::setId (const size_t id) {
    if (type == ControllerType::Gamepad) {
        gamepad.sf_joystick_index = id;
    }
}

void Controller::calibrate (Calibration* cali) {
    if (type == ControllerType::Gamepad) {
        gamepad.calibrate (cali);
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

bool Controller::fire() {
    switch (type) {
    case ControllerType::Gamepad:
        return gamepad.fire();
        break;
    case ControllerType::Keyboard:
        break;
    }
    return false;
}
bool Controller::anyInput() {
    if( 
    state.last_event!=ButtonEvent::None || vec_magnitude(state.dpad_vector) || vec_magnitude(state.left_stick_vector) || vec_magnitude(state.right_stick_vector)) 
        return true;
    return false;
}
} // namespace ss
