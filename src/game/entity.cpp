#include "entity.h"
#include "../math/vector.hpp"
#include <iostream>

namespace ss {
namespace engine {

void simulate (Movable* movable, const float dt) {
    switch (movable->type) {
    //
    // 2D movable is a player
    //
    case Movable::Two_D: {
        auto entity = &movable->movable2;

        entity->acceleration = entity->applied_force; // acceleration is applied by controller input
        entity->velocity += entity->acceleration;     // TODO limit to player top speed attribute

        // resistance
        sf::Vector2 resistance = vec_reversed (entity->velocity);
        resistance *= 0.05f;
        entity->velocity += resistance;
        entity->position = entity->position + entity->velocity;

        // damp very low velocities
        static const float DAMP_VELOCITY = 0.1f;
        if (less_than (vec_magnitude (entity->velocity), DAMP_VELOCITY)) {
            vec_reset (entity->velocity);
        }
        // reset force for next frames input
        vec_reset (entity->applied_force);

        // set the heading if there is a velocity
        if (vec_magnitude (entity->velocity)) {
            entity->heading = vec_normalized (entity->velocity);
        }
    }
    break;
    //
    // 3D movable is the ball
    //
    case Movable::Three_D:
        auto entity = &movable->movable3;


        entity->acceleration = entity->applied_force; // acceleration is applied by controller input
        entity->velocity += entity->acceleration;     // TODO limit to player top speed attribute

        // resistance
        sf::Vector3 resistance = vec_reversed (entity->velocity);
        resistance *= 0.05f;
        entity->velocity += resistance;
        entity->position = entity->position + entity->velocity;

        // damp very low velocities
        static const float DAMP_VELOCITY = 0.1f;
        if (less_than (vec_magnitude (entity->velocity), DAMP_VELOCITY)) {
            vec_reset (entity->velocity);
        }
        // reset force for next frames input
        vec_reset (entity->applied_force);

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
