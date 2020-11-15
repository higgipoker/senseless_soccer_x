#include "Movable.hpp"
#include "../math/math.hpp"
#include "../math/vector.hpp"

namespace ss {
namespace game {

static const float GRAVITATIONAL_CONSTANT = 9.8F;
void Movable::setPosition (const sf::Vector3f pos) {
    position = pos;
}

void Movable::update (const float dt) {
    // bounce?
    if (Math::less_than (velocity.z, 0) && Math::less_than (position.z, 0)) {
        velocity.z *= co_bounciness;
        // dampen for infinite bounce
        if (Math::less_than (fabsf (position.z), TOL)) {
            position.z = velocity.z = 0;
        }
    }
    integrate (dt);

    // damp velocity
    if (Math::less_than (Vector::magnitude (velocity), TOL)) {
        velocity.x = velocity.y = velocity.z = 0;
    }

    // decay spin
    forces.rotation *= co_spin_decay;
    if (Math::abs_less_than (Vector::magnitude (forces.rotation), TOL)) {
        forces.rotation.x = forces.rotation.y = forces.rotation.z = 0;
    }
}

void Movable::euler (float dt) {
    sf::Vector3f acceleration1 = integrate (dt);
    sf::Vector3f acceleration2 = integrate (dt);
    velocity = velocity + (acceleration1 + acceleration2) / 2.0F;
    position = position + velocity;
}

sf::Vector3f Movable::integrate (const float dt) {
    if (Math::greater_than (position.z, 0) && effected_by_gravity) {
        // gravity
        sf::Vector3f gravity;
        gravity.z = -GRAVITATIONAL_CONSTANT;
        forces.kinetic += gravity * mass * dt;

        // air drag = (air density * co_drag * cross section area) / 2 in the opposite direction to velocity
        sf::Vector3f drag = velocity;
        Vector::reverse (drag);
        Vector::normalize (drag);
        drag = drag * (air_density * co_air_friction * (PI * cross_section * cross_section) / 2);
        forces.kinetic += drag * dt;

    } else {
        // firction
        if (Math::greater_than (Vector::magnitude2d (velocity), 0)) {
            sf::Vector3f friction = velocity;
            Vector::reverse (friction);
            friction = friction * co_friction;
            velocity = velocity + friction;
        }

        // spin on the ground TODO
    }

    // return acceleration (f=ma)
    return forces.kinetic  + forces.rotation / mass;
}

} // namespace game
} // namespace ss
