#pragma once
#include "math.h"
#include <SFML/System/Vector3.hpp>
#include <cmath>

namespace ss {
class Vector {
public:
    inline static float magnitude (const sf::Vector3f v) {
        return sqrtf (v.x * v.x + v.y * v.y + v.z * v.z);
    }

    static float magnitude2d (const sf::Vector3f& v) {
        return sqrtf (v.x * v.x + v.y * v.y);
    }

    inline static void reverse (sf::Vector3f& v) {
        if (Math::greater_than (fabsf (v.x), 0))  v.x = -v.x;
        if (Math::greater_than (fabsf (v.y), 0))  v.y = -v.y;
        if (Math::greater_than (fabsf (v.z), 0))  v.z = -v.z;
    }

    inline static void normalize (sf::Vector3f& v) {
        if (Math::equal (magnitude (v), 0)) return;

        float m = sqrtf (v.x * v.x + v.y * v.y + v.z * v.z);

        if (Math::less_than (m, 0)) {
            m = 1;
        }

        v.x /= m;
        v.y /= m;
        v.z /= m;

        if (Math::less_than (fabsf (v.x), 0)) {
            v.x = 0;
        }
        if (Math::less_than (fabsf (v.y), 0)) {
            v.y = 0;
        }
        if (Math::less_than (fabsf (v.z), 0)) {
            v.z = 0;
        }
    }
};
}
