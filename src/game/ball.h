#pragma once
#include "entity.h"
#include "Movable.hpp"
#include <SFML/Graphics/CircleShape.hpp>

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
    float radius = 32;
    sf::CircleShape collider;
};

void init(Ball* ball, engine::MatchEngine* engine);
void update (Ball* ball, engine::MatchEngine* engine);

} // namespace
} // namespace
