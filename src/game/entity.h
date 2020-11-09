#pragma once
#include "movable.h"
#include "renderable.h"
#include "controllable.h"

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
