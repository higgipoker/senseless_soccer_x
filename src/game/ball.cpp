#include "ball.h"
#include "engine.h"
#include "../math/vector.hpp"

namespace ss {
namespace ball {

void init (Ball* ball, engine::MatchEngine* engine) {
    engine->sprites.movable[ball->id].type = engine::Movable::Three_D;
    ball->collider.setRadius (ball->radius);
    ball->collider.setOrigin ({ball->radius, ball->radius});
}

void update (Ball* ball, engine::MatchEngine* engine) {
    auto movable = &engine->sprites.movable[ball->id].movable3;
    auto sprite  = &engine->sprites.drawable[ball->id].sprite;
    ball->collider.setPosition (vec_to_2d (movable->position));
    sprite->setPosition (vec_to_2d(movable->position));
    sprite->setRotation (vec_angle ({movable->velocity.x, -movable->velocity.y}) +90);

}

} // namespace
} // namespace
