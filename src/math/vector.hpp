#pragma once
#include "math.hpp"
#include <SFML/System/Vector3.hpp>
#include <cmath>

namespace ss {
inline static float vec_magnitude (const sf::Vector3f v) {
    return sqrtf (v.x * v.x + v.y * v.y + v.z * v.z);
}

static float vec_magnitude2d (const sf::Vector3f& v) {
    return sqrtf (v.x * v.x + v.y * v.y);
}

inline static sf::Vector3f vec_reversed (const sf::Vector3f& vec) {
    sf::Vector3f v = vec;
    if (greater_than (fabsf (v.x), 0))  v.x = -v.x;
    if (greater_than (fabsf (v.y), 0))  v.y = -v.y;
    if (greater_than (fabsf (v.z), 0))  v.z = -v.z;
    return v;
}

inline static sf::Vector3f vec_normalized (sf::Vector3f& vec) {
    if (equal (vec_magnitude (vec), 0)) return vec;

    sf::Vector3f v = vec;

    float m = sqrtf (v.x * v.x + v.y * v.y + v.z * v.z);

    if (less_than (m, 0)) {
        m = 1;
    }

    v.x /= m;
    v.y /= m;
    v.z /= m;

    if (less_than (fabsf (v.x), 0)) {
        v.x = 0;
    }
    if (less_than (fabsf (v.y), 0)) {
        v.y = 0;
    }
    if (less_than (fabsf (v.z), 0)) {
        v.z = 0;
    }
    return v;
}
};
