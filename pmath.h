#ifndef PMATH_H
#define PMATH_H
#include <cmath>
#include <array>

constexpr auto NEAR_ZERO(float x) -> bool{
    return (x < 0.001 && x > -0.001);
}

constexpr auto IN_RANGE(float x, float min, float max) -> bool{
    return (x >= min && x <= max);
}


constexpr auto degree_to_radian(float degree) -> float{
    return degree * M_PI / 180;
}

constexpr auto radian_to_degree(float radian) -> float{
    return radian * 180 / M_PI;
}

enum class P_DIRECTION{
    P_UP = 0,
    P_DOWN = 180,
    P_LEFT = 90,
    P_RIGHT = -90,
    P_NONE = 1000,
};

// angle in degree
// return Direction of angle
constexpr auto angle_to_direction(float angle) -> P_DIRECTION{
    if(IN_RANGE(angle, -180.0f, -90.0f)){
        return P_DIRECTION::P_RIGHT;
    }else if(IN_RANGE(angle, -90.0f, 0.0f)){
        return P_DIRECTION::P_UP;
    }else if(IN_RANGE(angle, 0.0f, 90.0f)){
        return P_DIRECTION::P_LEFT;
    }else if(IN_RANGE(angle, 90.0f, 180.0f)){
        return P_DIRECTION::P_DOWN;
    }
    return P_DIRECTION::P_NONE;
}

constexpr std::array<float, 2> normalize(const std::array<float, 2>& vec) {
    float length = std::sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
    return {vec[0] / length, vec[1] / length};
}






#endif
