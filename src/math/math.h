#pragma once
#include <cmath>
namespace ss {
const float TOL = 0.01F;
const float PI = 3.14159265359F;

class Math {
public:
    static float Radians (float a) {
        return a * (PI / 180.0F);
    }

    static float Degrees (float a) {
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
} // namespace ss
