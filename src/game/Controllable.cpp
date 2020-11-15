#include "Controllable.hpp"
#include "Entity.hpp"
namespace ss {
namespace game {
void Controllable::update() {
    if (controller) {
        // check controller states here
    }
}
void Controllable::attachController(Gamepad* c){
    controller = c;
}
void Controllable::detatchController(){
    controller = nullptr;
}
}// namespace game
}// namespace ss
