#pragma once
#include "../graphics/Sprite.hpp"
#include "../input/InputDevice.hpp"

namespace ss {
namespace game {
    
class Player {
public:
    void update();
    void attachController (InputDevice* input);
    void connectSprite(Sprite* s){sprite = s;}
private:
    Sprite* sprite = nullptr;
    InputDevice* controller = nullptr;
};
}// namespace game
}// namespace ss
