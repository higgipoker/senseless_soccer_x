#pragma once
#include "Movable.h"
#include "Renderable.h"
#include "Controllable.h"

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