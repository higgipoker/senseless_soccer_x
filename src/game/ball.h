#pragma once
#include "entity.h"
#include "Movable.hpp"

namespace ss {
namespace engine {
struct MatchEngine;
}
namespace ball {

// ********************
// Ball
// ********************
struct Ball {
    int id = -1;
    engine::Movable movable;
};
void init(Ball* ball);
void update (Ball* ball, engine::MatchEngine* engine);
void simulate (Ball* ball, const float dt);

} // namespace
} // namespace
