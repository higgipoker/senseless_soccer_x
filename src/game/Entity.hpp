#pragma once
#include "Movable.hpp"
#include "../graphics/Sprite.hpp"

namespace ss {
namespace game {

class Entity {
public:
    Entity();
    Movable     movable;
    Sprite      sprite;

private:
};

} // namespace game
} // namespace ss
