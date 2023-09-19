#ifndef PMATH_H
#define PMATH_H
#include <array>
#include <cmath>
#include <iostream>
#include <random>

#include "ppoint2d.h"

constexpr auto NEAR_ZERO(float x) -> bool { return (x < 0.001 && x > -0.001); }

constexpr auto IN_RANGE(float x, float min, float max) -> bool { return (x >= min && x <= max); }

constexpr auto degree_to_radian(float degree) -> float { return degree * M_PI / 180; }

constexpr auto radian_to_degree(float radian) -> float { return radian * 180 / M_PI; }

constexpr auto EPSILON = std::numeric_limits<float>::epsilon();

class PInterval {
   public:
	class Iterator {
	   public:
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = float;
		using value_type = float;
		using pointer = float*;
		using reference = float&;

		Iterator(float value) : _value(value) {}

		float operator*() const { return _value; }

		Iterator& operator++() {
			_value += 0.001;
			return *this;
		}

		Iterator operator++(int) {
			Iterator tmp(*this);
			operator++();
			return tmp;
		}

		Iterator& operator--() {
			_value -= 0.001;
			return *this;
		}

		Iterator operator--(int) {
			Iterator tmp(*this);
			operator--();
			return tmp;
		}

		bool operator==(const Iterator& other) const { return _value == other._value; }

		bool operator!=(const Iterator& other) const { return !(*this == other); }

	   private:
		float _value;
	};

	Iterator begin() const { return Iterator(_min); }

	Iterator end() const {
		return Iterator(_max + 0.001);	// +0.001, um sicherzustellen, dass _max auch berücksichtigt wird
	}

   public:
	class ConstIterator {
	   public:
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = float;
		using value_type = float;
		using pointer = const float*;
		using reference = const float&;

		ConstIterator(float value) : _value(value) {}

		float operator*() const { return _value; }

		ConstIterator& operator++() {
			_value += 0.001;
			return *this;
		}

		ConstIterator operator++(int) {
			ConstIterator tmp(*this);
			operator++();
			return tmp;
		}

		ConstIterator& operator--() {
			_value -= 0.001;
			return *this;
		}

		ConstIterator operator--(int) {
			ConstIterator tmp(*this);
			operator--();
			return tmp;
		}

		bool operator==(const ConstIterator& other) const { return _value == other._value; }

		bool operator!=(const ConstIterator& other) const { return !(*this == other); }

	   private:
		float _value;
	};

	ConstIterator cbegin() const { return ConstIterator(_min); }

	ConstIterator cend() const { return ConstIterator(_max + 0.001); }

   public:
	PInterval(float min, float max) : _min(min), _max(max) {
		_gen = std::mt19937(_rd());
		_dis = std::uniform_real_distribution<float>(_min, _max);
	}
	PInterval() : PInterval(0, 0) {}
	PInterval(const PInterval& other) : PInterval(other._min, other._max) {}
	PInterval(PInterval&& other) noexcept : PInterval(other._min, other._max) {}

	auto operator=(const PInterval& other) -> PInterval& {
		_min = other._min;
		_max = other._max;
		return *this;
	}

	auto operator=(PInterval&& other) noexcept -> PInterval& {
		_min = other._min;
		_max = other._max;
		return *this;
	}

	auto min() const -> float { return _min; }
	auto max() const -> float { return _max; }

	auto pick_random() -> float { return _dis(_gen); }

	auto operator+(const PInterval& a) const -> PInterval { return PInterval(_min + a._min, _max + a._max); }

	auto operator-(const PInterval& a) const -> PInterval { return PInterval(_min - a._min, _max - a._max); }

	auto operator*(const PInterval& a) const -> PInterval {
		float min1 = _min * a._min;
		float min2 = _min * a._max;
		float max1 = _max * a._min;
		float max2 = _max * a._max;
		return PInterval(std::min(std::min(min1, min2), std::min(max1, max2)),
						 std::max(std::max(min1, min2), std::max(max1, max2)));
	}

	auto operator/(const PInterval& a) const -> PInterval {
		if (a._min > 0 || a._max < 0) {
			float min1 = _min / a._min;
			float min2 = _min / a._max;
			float max1 = _max / a._min;
			float max2 = _max / a._max;
			return PInterval(std::min(std::min(min1, min2), std::min(max1, max2)),
							 std::max(std::max(min1, min2), std::max(max1, max2)));
		} else if (a._min == 0 && a._max == 0) {
			throw std::runtime_error("Division by zero interval is not allowed");
		} else if (a._min == 0) {
			return PInterval(-std::numeric_limits<float>::infinity(), _max / a._max);
		} else if (a._max == 0) {
			return PInterval(_min / a._min, std::numeric_limits<float>::infinity());
		} else {
			return PInterval(-std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
		}
	}

	auto in(float x) const -> bool { return (x >= _min && x <= _max); }

   private:
	float _min;
	float _max;
	std::random_device _rd;
	std::mt19937 _gen;
	std::uniform_real_distribution<float> _dis;
};

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
constexpr auto rotate_point(float xc, float yc, float x, float y, float rad) -> PPoint2D {
	const float xr1 = (x - xc) * cos(rad) + (y - yc) * sin(rad) + xc;
	const float yr1 = -(x - xc) * sin(rad) + (y - yc) * cos(rad) + yc;
	return {xr1, yr1};
}

constexpr auto distance_between_points(const PPoint2D& p1, const PPoint2D& p2) -> float {
	return std::sqrt(std::pow(p1[0] - p2[0], 2) + std::pow(p1[1] - p2[1], 2));
}

#endif
