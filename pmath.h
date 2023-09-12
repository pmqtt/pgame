#ifndef PMATH_H
#define PMATH_H
#include <array>
#include <cmath>
#include <iostream>

constexpr auto NEAR_ZERO(float x) -> bool { return (x < 0.001 && x > -0.001); }

constexpr auto IN_RANGE(float x, float min, float max) -> bool { return (x >= min && x <= max); }

constexpr auto degree_to_radian(float degree) -> float { return degree * M_PI / 180; }

constexpr auto radian_to_degree(float radian) -> float { return radian * 180 / M_PI; }

constexpr auto EPSILON = std::numeric_limits<float>::epsilon();


// angle in degree
// return Direction of angle
enum class P_DIRECTION {
	P_UP = 0,
	P_DOWN = 180,
	P_LEFT = 90,
	P_RIGHT = -90,
	P_LEFT_DOWN,
	P_LEFT_UP,
	P_RIGHT_DOWN,
	P_RIGHT_UP,
	P_NONE = 1000,
};

// angle in degree
// return Direction of angle
constexpr auto angle_to_direction(float angle) -> P_DIRECTION {
	if (IN_RANGE(angle, -180.0f, -90.0f)) {
		return P_DIRECTION::P_RIGHT;
	} else if (IN_RANGE(angle, -90.0f, 0.0f)) {
		return P_DIRECTION::P_UP;
	} else if (IN_RANGE(angle, 0.0f, 90.0f)) {
		return P_DIRECTION::P_LEFT;
	} else if (IN_RANGE(angle, 90.0f, 180.0f)) {
		return P_DIRECTION::P_DOWN;
	}
	return P_DIRECTION::P_NONE;
}

constexpr auto vec_to_direction(float nx, float ny) -> P_DIRECTION {
	if (nx == 0 && ny >= 1) {
		return P_DIRECTION::P_DOWN;
	} else if (nx == 0 && ny <= -1) {
		return P_DIRECTION::P_UP;
	} else if (nx <= -1 && ny == 0) {
		return P_DIRECTION::P_LEFT;
	} else if (nx >= 1 && ny == 0) {
		return P_DIRECTION::P_RIGHT;
	} else if (nx > 0 && ny > 0) {
		return P_DIRECTION::P_RIGHT_DOWN;
	} else if (nx < 0 && ny > 0) {
		return P_DIRECTION::P_LEFT_DOWN;
	} else if (nx < 0 && ny < 0) {
		return P_DIRECTION::P_LEFT_UP;
	} else if (nx > 0 && ny < 0) {
		return P_DIRECTION::P_RIGHT_UP;
	}

	return P_DIRECTION::P_NONE;
}

constexpr auto normalize(const std::array<float, 2>& vec) -> std::array<float, 2> {
	float length = std::sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
	std::array<float, 2> res = {vec[0] / length, vec[1] / length};
	return res;
}

constexpr auto angle_between_vectors(double vx, double vy, double nx, double ny) -> double {
	double dotProduct = vx * nx + vy * ny;
	double lengthV = std::sqrt(vx * vx + vy * vy);
	double lengthN = std::sqrt(nx * nx + ny * ny);
	double angle = std::acos(dotProduct / (lengthV * lengthN));
	return angle;
}

// calculate the position of a point after rotation
// xc, yc: center of rotation
// x, y: point to rotate
// rad: angle in radian
constexpr auto rotate_point(float xc, float yc, float x, float y, float rad) -> std::array<float, 2> {
	const float xr1 = (x - xc) * cos(rad) + (y - yc) * sin(rad) + xc;
	const float yr1 = -(x - xc) * sin(rad) + (y - yc) * cos(rad) + yc;
	return {xr1, yr1};
}

constexpr auto distance_between_points(const std::array<float, 2>& p1, const std::array<float, 2>& p2) -> float {
	return std::sqrt(std::pow(p1[0] - p2[0], 2) + std::pow(p1[1] - p2[1], 2));
}

#endif
