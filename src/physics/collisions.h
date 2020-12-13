#pragma once
#include "../math/math.hpp"
#include <SFML/Graphics/CircleShape.hpp>

namespace ss {
namespace physics {
    
// NOTE: assumes origin has been set to center!!
inline static bool collides (const sf::CircleShape* c1, const sf::CircleShape* c2) {
    // One of the circles has no size
    if (equal (c1->getRadius(), 0) || equal (c2->getRadius(), 0)) {
        return false;
    }

    // compare the distance to combined radii
    float dx = c2->getPosition().x - c1->getPosition().x;
    float dy = c2->getPosition().y - c1->getPosition().y;
    float radii = c1->getRadius() + c2->getRadius();

    if ( (dx * dx) + (dy * dy) < radii * radii) {
        return true;
    }
    return false;
}

}
}
