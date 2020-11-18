#pragma once
#include <cmath>
namespace ss {
inline static constexpr float TOL = 0.01F;
inline static constexpr float PI = static_cast<float> (M_PI);

inline static constexpr float radians (float a) {
    return a * (PI / 180.0F);
}

inline static constexpr float degrees (float a) {
    return a * (180.0F / PI);
}

inline static bool equal (const float a, const float b, const float epsilon = TOL) {
    return (std::fabs (a - b) <= epsilon);
}

inline static bool greater_than (const float a, const float b, const float epsilon = TOL) {
    return (a - b) > ( (std::fabs (a) < std::fabs (b) ? std::fabs (b) : std::fabs (a)) * epsilon);
}

inline static bool less_than (const float a, const float b, const float epsilon = TOL) {
    return (b - a) > ( (std::fabs (a) < std::fabs (b) ? std::fabs (b) : std::fabs (a)) * epsilon);
}

inline static bool abs_less_than (const float a, const float b, const float epsilon = TOL) {
    return (b - std::fabs (a)) >
           ( (std::fabs (a) < std::fabs (b) ? std::fabs (b) : std::fabs (a)) * epsilon);
}
};
