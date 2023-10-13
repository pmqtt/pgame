#ifndef PVECTOR2D_H
#define PVECTOR2D_H
#include <array>
#include <cmath>
#include <iostream>

class PVector2D {
   public:
	constexpr PVector2D(const std::array<float, 2>& point) : _point(point) {}
	constexpr PVector2D(float x, float y) : _point{x, y} {}
	constexpr PVector2D() : _point{0, 0} {}

	constexpr auto x() const -> float { return _point[0]; }
	constexpr auto y() const -> float { return _point[1]; }

	constexpr void x(float x) { _point[0] = x; }
	constexpr void y(float y) { _point[1] = y; }

	constexpr auto operator[](std::size_t i) -> float& { return _point[i]; }
	constexpr auto operator[](std::size_t i) const -> float { return _point[i]; }

	constexpr auto operator+(const PVector2D& a) const -> PVector2D {
		return PVector2D(_point[0] + a._point[0], _point[1] + a._point[1]);
	}

	constexpr auto operator-(const PVector2D& a) const -> PVector2D {
		return PVector2D(_point[0] - a._point[0], _point[1] - a._point[1]);
	}

	constexpr auto dot(const PVector2D& a) const -> float { return _point[0] * a._point[0] + _point[1] * a._point[1]; }

	constexpr auto cross(const PVector2D& a) const -> float { return _point[0] * a._point[1] - _point[1] * a._point[0]; }

	constexpr auto operator*(float a) const -> PVector2D { return PVector2D(_point[0] * a, _point[1] * a); }

	constexpr auto operator/(float a) const -> PVector2D { return PVector2D(_point[0] / a, _point[1] / a); }

	constexpr auto operator+=(const PVector2D& a) -> PVector2D& {
		_point[0] += a._point[0];
		_point[1] += a._point[1];
		return *this;
	}

	constexpr auto operator-=(const PVector2D& a) -> PVector2D& {
		_point[0] -= a._point[0];
		_point[1] -= a._point[1];
		return *this;
	}

	constexpr auto operator*=(float a) -> PVector2D& {
		_point[0] *= a;
		_point[1] *= a;
		return *this;
	}

	constexpr auto operator/=(float a) -> PVector2D& {
		_point[0] /= a;
		_point[1] /= a;
		return *this;
	}

	constexpr auto operator==(const PVector2D& a) const -> bool {
		return _point[0] == a._point[0] && _point[1] == a._point[1];
	}

	constexpr auto operator!=(const PVector2D& a) const -> bool {
		return _point[0] != a._point[0] || _point[1] != a._point[1];
	}

	constexpr auto length() const -> float { return std::sqrt(_point[0] * _point[0] + _point[1] * _point[1]); }

    constexpr auto quad_length()const -> float { return _point[0] * _point[0] + _point[1] * _point[1]; }

	constexpr auto normalize() -> PVector2D& {
		float l = length();
		_point[0] /= l;
		_point[1] /= l;
		return *this;
	}

	constexpr auto normalized() const -> PVector2D {
		float l = length();
		return PVector2D(_point[0] / l, _point[1] / l);
	}

	constexpr auto distance(const PVector2D& a) const -> float {
		return std::sqrt((_point[0] - a._point[0]) * (_point[0] - a._point[0]) +
						 (_point[1] - a._point[1]) * (_point[1] - a._point[1]));
	}

	constexpr auto angle() const -> float { return std::atan2(_point[1], _point[0]); }

	friend auto operator<<(std::ostream& os, const PVector2D& p) -> std::ostream& {
		os << "(x, y):=";
		os << "(" << p._point[0] << ", " << p._point[1] << ")";
		return os;
	}

   private:
	std::array<float, 2> _point;
};

#endif
