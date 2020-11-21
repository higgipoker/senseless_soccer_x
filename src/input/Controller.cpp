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
} // namespace game
} // namespace ss
