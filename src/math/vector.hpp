#pragma once
#include "math.hpp"
#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <string>

namespace ss {
inline static std::string vec_print (const sf::Vector3f& v) {
    return {std::to_string (v.x) + ", " + std::to_string (v.y) };
}

inline static std::string vec_print (const sf::Vector2f v) {
    return {std::to_string (v.x) + ", " + std::to_string (v.y) };
}

inline static void vec_reset (sf::Vector3f& v) {
    v.x = v.y = v.z = 0;
}

inline static void vec_reset(sf::Vector2f& v){
    v.x = v.y = 0;
}

inline static void vec_dampen (sf::Vector3f& v) {
    if (less_than_or_equal (fabsf (v.x), 0)) v.x = 0;
    if (less_than_or_equal (fabsf (v.y), 0)) v.y = 0;
    if (less_than_or_equal (fabsf (v.z), 0)) v.z = 0;
}

inline static float vec_magnitude (const sf::Vector3f v) {
    return sqrtf (v.x * v.x + v.y * v.y + v.z * v.z);
}

inline static float vec_magnitude (const sf::Vector2f v) {
    return sqrtf (v.x * v.x + v.y * v.y);
}

inline static void vec_set_magnitude(sf::Vector2f& v, const float mag){
    float old_mag = vec_magnitude(v);
    v.x = v.x * mag / old_mag;
    v.y = v.y * mag / old_mag;
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

inline static sf::Vector3f vec_normalized (const sf::Vector3f& vec) {
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

inline static sf::Vector2f vec_normalized (const sf::Vector2f& vec) {
    if (equal (vec_magnitude (vec), 0)) return vec;

    sf::Vector2f v = vec;

    float m = sqrtf (v.x * v.x + v.y * v.y);

    if (less_than (m, 0)) {
        m = 1;
    }

    v.x /= m;
    v.y /= m;

    if (less_than (fabsf (v.x), 0)) {
        v.x = 0;
    }
    if (less_than (fabsf (v.y), 0)) {
        v.y = 0;
    }

    return v;
}

inline static sf::Vector3f vec_normalized2d (const sf::Vector3f v) {
    sf::Vector3f vec = v;
    vec.z = 0;
    vec = vec_normalized (vec);
    vec.z = 0;
    vec_dampen (vec);
    return vec;
}

inline static void vec_rotate (sf::Vector3f& v, const float a, const float x_origin = 0, const float y_origin = 0) {
    const float rads = radians (a);

    v.x -= x_origin;
    v.y -= y_origin;

    float nx = cosf (rads) * v.x - sinf (rads) * v.y;
    float ny = sinf (rads) * v.x + cosf (rads) * v.y;

    v.x = nx;
    v.y = ny;

    v.x += x_origin;
    v.y += y_origin;

    vec_dampen (v);
}

inline static float vec_angle (const sf::Vector3f& vec) {
    sf::Vector3f normalized = vec_normalized (vec);
    float rads = atan2f (normalized.x,  normalized.y);
    return degrees (rads);
}

inline static void vec_rotateTowards (sf::Vector3f& v, const sf::Vector3f& target, const float degrees) {
    if (v == target) return;
    auto current_angle = vec_angle (v);
    auto target_angle = vec_angle (target);

    if (less_than (fabsf (current_angle - target_angle), degrees)) {
        v = target;
    } else {
        if (abs_less_than (current_angle, target_angle)) {
            if (std::abs (current_angle - target_angle) < 180) {
                vec_rotate (v, degrees);
            } else {
                vec_rotate (v, -degrees);
            }
        } else if (greater_than (current_angle, target_angle)) {
            if (std::abs (current_angle - target_angle) < 180) {
                vec_rotate (v, -degrees);
            } else {
                vec_rotate (v, degrees);
            }
        }
    }
    vec_dampen (v);
}

inline static sf::Vector2f vec_multiply(const sf::Vector2f& v1, const sf::Vector2f& v2){
    return {v1.x*v2.x, v1.y*v2.y};
}
};
