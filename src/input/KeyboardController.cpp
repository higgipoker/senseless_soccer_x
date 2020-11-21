#include "KeyboardController.hpp"
#include "../math/vector.hpp"
#include <SFML/Window/Keyboard.hpp>
using namespace sf;
namespace ss {
void  ss::KeyboardController::update (ss::ControllerState& s) {
    vec_reset(s.left_stick_vector);
    // TODO configurable buttons
    if(Keyboard::isKeyPressed(Keyboard::A)){
        s.left_stick_vector.x = -1;
    }
    
    if(Keyboard::isKeyPressed(Keyboard::D)){
        s.left_stick_vector.x = 1;
    }
    
    if(Keyboard::isKeyPressed(Keyboard::W)){
        s.left_stick_vector.y = -1;
    }
    
    if(Keyboard::isKeyPressed(Keyboard::S)){
        s.left_stick_vector.y = 1;
    }

}
}

