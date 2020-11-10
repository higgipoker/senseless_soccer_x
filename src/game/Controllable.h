#pragma once
#include "../input/Gamepad.h"
namespace ss {
namespace game {

class Entity;
class Controllable {
public:
    void update();
    void attachController (Gamepad* c);
    void detatchController();
private:
    Entity* entity = nullptr;
    Gamepad* controller = nullptr;
};
}// namespace game
}// namespace ss
