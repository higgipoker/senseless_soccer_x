#pragma once
#include "Movable.hpp"
#include "Renderable.hpp"
#include "Controllable.hpp"

namespace ss {
namespace game {

class Entity {
public:
    Entity();
    Movable         movable;
    Renderable      renderable;
    Controllable    controllable;

private:
};

} // namespace game
} // namespace ss
