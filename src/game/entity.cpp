#include "entity.h"
#include "../math/vector.hpp"
#include <iostream>

namespace ss {
namespace engine {

void simulate (Movable* movable, const float dt) {
    switch (movable->type) {
    case Movable::Two_D: {
        
        movable->movable2.acceleration = movable->movable2.applied_force; // acceleration is applied by controller input
        movable->movable2.velocity += movable->movable2.acceleration;                              // TODO limit to player top speed attribute

        // resistance
        sf::Vector2 resistance = vec_reversed (movable->movable2.velocity);
        resistance *= 0.05f;
        movable->movable2.velocity += resistance;

        movable->movable2.position = movable->movable2.position + movable->movable2.velocity;

        // damp very low velocities
        static const float DAMP_VELOCITY = 0.1f;
        if (less_than (vec_magnitude (movable->movable2.velocity), DAMP_VELOCITY)) {
            vec_reset (movable->movable2.velocity);
        }
        // reset force for next frames input
        vec_reset (movable->movable2.applied_force);
    }
    break;
    case Movable::Three_D:
        break;
    }
}

sf::Vector2f position (Movable* movable) {
    switch (movable->type) {
    case Movable::Two_D:
        return movable->movable2.position;
    case Movable::Three_D:
        return {movable->movable3.position.x, movable->movable3.position.y};
        break;
    }

    return {0, 0};
}

}// namespace ss
}// namespace engine
