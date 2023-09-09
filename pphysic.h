#ifndef PPHYSIC_H
#define PPHYSIC_H
#include <SDL2/SDL.h>

#include <tuple>

#include "pcolider.h"
#include "pmath.h"
#include "pprimitive.h"

class PPhysicObject;

static int id = 0;

struct PColision {
	PColision(const std::array<float, 2>& point, float direction_angle, std::shared_ptr<PPhysicObject> colide_with,
			  const std::array<float, 2>& normals)
		: point(point), direction_angle(direction_angle), colide_with(colide_with), normals(normals), handled(false) {
		name = "colision_" + std::to_string(id++);
	}
	std::array<float, 2> point;
	float direction_angle;
	std::shared_ptr<PPhysicObject> colide_with;
	std::array<float, 2> normals;
	bool handled;
	std::string name;
};

class PPhysicObject {
   public:
	PPhysicObject(std::shared_ptr<PDrawable> drawable)
		: _drawable(drawable),
		  _gravity(0),
		  _velocity{0, 0},
		  _velocity_direction(0),
		  _restition(1),
		  _acceleration(0),
		  _colision(nullptr) {
		_colide = false;
	}

	PPhysicObject(std::shared_ptr<PDrawable> drawable, std::shared_ptr<PColider> colider)
		: _drawable(drawable),
		  _gravity(0.0),
		  _velocity{0, 0},
		  _velocity_direction(0),
		  _restition(1),
		  _colider(colider),
		  _colision(nullptr) {
		_colide = false;
	}

	virtual ~PPhysicObject() = default;

	// Set the gravity for this object in pixels per second
	void gravity(float gravity) { _gravity = gravity; }

	// Set the velocity for this object in pixels per second
	void velocity(float x, float y) {
		_velocity[0] = x;
		_velocity[1] = y;
	}

	void velocity(const std::array<float, 2>& v) {
		_velocity[0] += v[0];
		_velocity[1] += v[1];
	}

	auto velocity() const -> PPoint2D { return _velocity; }

	void acceleration(float value) {
		_acceleration = value;
		;
	}

	// Set the velocity direction for this object in degrees
	auto velocity_direction() -> float {
		const float rad = atan2(_velocity[1], _velocity[0]);
		return radian_to_degree(rad);
	}

	void restitution(float value) { _restition = value; }

	auto collision_detected() const -> bool { return _colide; }

	auto dir_to_string(P_DIRECTION dir) const -> std::string {
		switch (dir) {
			case P_DIRECTION::P_UP:
				return "UP";
			case P_DIRECTION::P_DOWN:
				return "DOWN";
			case P_DIRECTION::P_LEFT:
				return "LEFT";
			case P_DIRECTION::P_RIGHT:
				return "RIGHT";
			case P_DIRECTION::P_LEFT_UP:
				return "UP_LEFT";
			case P_DIRECTION::P_RIGHT_UP:
				return "UP_RIGHT";
			case P_DIRECTION::P_LEFT_DOWN:
				return "DOWN_LEFT";
			case P_DIRECTION::P_RIGHT_DOWN:
				return "DOWN_RIGHT";
			default:
				return "NONE";
		}
	}

	auto is_direction(P_DIRECTION dir1, P_DIRECTION dir2) const -> bool {
		if (dir2 == P_DIRECTION::P_UP) {
			if (dir1 == P_DIRECTION::P_UP || dir1 == P_DIRECTION::P_LEFT_UP || dir1 == P_DIRECTION::P_RIGHT_UP) {
				return true;
			}
		}
		if (dir2 == P_DIRECTION::P_DOWN) {
			if (dir1 == P_DIRECTION::P_DOWN || dir1 == P_DIRECTION::P_LEFT_DOWN || dir1 == P_DIRECTION::P_RIGHT_DOWN) {
				return true;
			}
		}
		if (dir2 == P_DIRECTION::P_LEFT) {
			if (dir1 == P_DIRECTION::P_LEFT || dir1 == P_DIRECTION::P_LEFT_UP || dir1 == P_DIRECTION::P_LEFT_DOWN) {
				return true;
			}
		}
		if (dir2 == P_DIRECTION::P_RIGHT) {
			if (dir1 == P_DIRECTION::P_RIGHT || dir1 == P_DIRECTION::P_RIGHT_UP || dir1 == P_DIRECTION::P_RIGHT_DOWN) {
				return true;
			}
		}
		return false;
	}

	auto is_point_equal(const PPoint2D& a, const PPoint2D& b) const -> bool {
		std::cout << "(" << a[0] << "," << a[1] << ") == "
				  << "(" << b[0] << "," << b[1] << ")"
				  << "\n";
		if (a[0] == b[0] && a[1] == b[1]) {
			return true;
		}
		return false;
	}

	auto collide_with_bottom_edge(const PPoint2D& a, const PPoint2D& b) const -> std::pair<bool, float> {
		if (_velocity[1] < 0) {
			const float y = a[1];
			const float m = _velocity[1] / _velocity[0];
			const float x = (y - a[1]) / m + a[0];
			if (x >= std::min(a[0], b[0]) && x <= std::max(a[0], b[0])) {
				return {true, x};
			}
		}
		return {false, 0};
	}

	auto collide_with_right_edge(const PPoint2D& a, const PPoint2D& b) const -> std::pair<bool, float> {
		if (_velocity[0] < 0) {
			const float x = a[0];
			const float m = _velocity[1] / _velocity[0];
			const float y = m * (x - a[0]) + a[1];
			if (y >= std::min(a[1], b[1]) && y <= std::max(a[1], b[1])) {
				return {true, y};
			}
		}
		return {false, 0};
	}

	auto collide_with_left_edge(const PPoint2D& a, const PPoint2D& b) const -> std::pair<bool, float> {
		if (_velocity[0] > 0) {
			const float x = a[0];
			const float m = _velocity[1] / _velocity[0];
			const float y = m * (x - a[0]) + a[1];
			if (y >= std::min(a[1], b[1]) && y <= std::max(a[1], b[1])) {
				return {true, y};
			}
		}
		return {false, 0};
	}

	auto collide_with_top_edge(const PPoint2D& a, const PPoint2D& b) const -> std::pair<bool, float> {
		if (_velocity[1] > 0) {
			const float y = a[1];
			const float m = _velocity[1] / _velocity[0];
			const float x = (y - a[1]) / m + a[0];
			if (x >= std::min(a[0], b[0]) && x <= std::max(a[0], b[0])) {
				return {true, x};
			}
		}
		return {false, 0.0};
	}

	auto intersection_with_side(const std::array<float, 2>& start, const std::array<float, 2>& end,
								const std::array<float, 2>& point, float m2, float c2) const -> PPoint2D {
		P_UNUSED(point);
		double dx = end[0] - start[0];
		double dy = end[1] - start[1];

		double m1 = (std::abs(dx) > 1e-6) ? dy / dx : 1e6;	// slope of the side
		double c1 = start[1] - m1 * start[0];				// y-intercept of the side

		if (std::abs(m1 - m2) < 1e-6) {	 // Parallel lines
			return {INFINITY, INFINITY};
		}

		double x = (c1 - c2) / (m2 - m1);
		double y = m2 * x + c2;

		std::cout << "(x,y) := (" << x << "," << y << ")"
				  << "\n";

		if (x >= std::min(start[0], end[0]) && x <= std::max(start[0], end[0]) && y >= std::min(start[1], end[1]) &&
			y <= std::max(start[1], end[1])) {
			return {(float)x, (float)y};
		}
		return {INFINITY, INFINITY};
	}

	auto find_nearst_edge(const PVertices2D& rect, const PPoint2D& p) const -> std::pair<int, int> {
		PVertices2D intersections;
		const PPoint2D point = _velocity;

		float m = (std::abs(_velocity[0]) > 1e-6) ? _velocity[1] / _velocity[0] : 1e6;	// slope of the line
		float c = p[1] - m * p[0];														// y-intercept

		intersections[0] = intersection_with_side(rect[0], rect[1], point, m, c);
		intersections[1] = intersection_with_side(rect[1], rect[2], point, m, c);
		intersections[2] = intersection_with_side(rect[2], rect[3], point, m, c);
		intersections[3] = intersection_with_side(rect[3], rect[0], point, m, c);

		std::pair<int, int> nearest_edge = {-1, -1};
		float min_distance = INFINITY;
		for (int i = 0; i < 4; ++i) {
			std::cout << " Inter: " << intersections[i][0] << " , " << intersections[i][1] << "\n";
			if (intersections[i][0] == INFINITY && intersections[i][1] == INFINITY) {
				continue;
			}
			const float distance = distance_between_points(intersections[i], p);
			if (distance < min_distance) {
				min_distance = distance;
				nearest_edge = {i, (i + 1) % 4};
			}
		}
		if (nearest_edge.first == 0 && nearest_edge.second == 1 && _velocity[1] > 0) {
			return nearest_edge;
		}

		if (nearest_edge.first == 1 && nearest_edge.second == 2 && _velocity[0] < 0) {
			return nearest_edge;
		}

		if (nearest_edge.first == 2 && nearest_edge.second == 3 && _velocity[1] < 0) {
			return nearest_edge;
		}

		if (nearest_edge.first == 3 && nearest_edge.second == 0 && _velocity[0] > 0) {
			return nearest_edge;
		}

		return {-1, -1};
	}

    auto collision_shape_index(const PVertices2D& rect, const PPoint2D& a, const PPoint2D &b ) const -> std::pair<int,int> {
        std::cout<<"rect:= { (" 
                 << rect[0][0] << ","<< rect[0][1]<<"),("
                 << rect[1][0] << ","<< rect[1][1]<<"),("
                 << rect[2][0] << ","<< rect[2][1]<<"),("
                 << rect[3][0] << ","<< rect[2][1]<<"),("<<"\n";
        std::cout<<"a[] = { "<<a[0]<<","<<a[1]<< "}\n";
        std::cout<<"b[] = { "<<b[0]<<","<<b[1]<< "}\n";
        int index1 = -1;
        for(int i = 0; i < 4; ++i) {
            if(rect[i][0] == a[0] && rect[i][1] == a[1]) {
                index1 = i;
            }
        }
        int index2 = -1;
        for(int i = 0; i < 4; ++i) {
            if(rect[i][0] == b[0] && rect[i][1] == b[1]) {
                index2 = i;
            }
        }
        return {index1, index2};
    }

	auto colide(std::shared_ptr<PPhysicObject> other) -> bool {
		if (_colider) {
			_colide = _colider->colide(_drawable, other->_drawable);
			if (_colide) {
				auto point = std::array<float, 2>{_drawable->x(), _drawable->y()};
				_colision = std::make_shared<PColision>(point, velocity_direction(), other, _colider->normals());
				return true;
			}
		}
		_colide = false;
		_colision = nullptr;
		return false;
	}

	void move(float delta_time) {
		// WORKAROUND: move the object a little bit more to avoid multiple colision with the same object
		const float correction_delta_time = 0.01;
		if (delta_time == 0) {
			return;
		}
		if (NEAR_ZERO(_velocity[0]) && NEAR_ZERO(_velocity[1])) {
			return;
		}
		if (_colide) {
			delta_time += correction_delta_time;
            auto mtv = _colider->mtv();
            if ((_velocity[0] * mtv[0] + _velocity[1] * mtv[1]) < 0) {
                mtv[0] = -mtv[0];
                mtv[1] = -mtv[1];
            }
            auto collision_normal = normalize(_colision->normals);
			const float dot_product = _velocity[0] * collision_normal[0] + _velocity[1] * collision_normal[1];
			_velocity[0] -= (1 + _restition) * dot_product * collision_normal[0];
			_velocity[1] -= (1 + _restition) * dot_product * collision_normal[1];
		} else {
			_velocity[1] += _gravity * delta_time;
			_velocity[0] += _acceleration * delta_time;
		}

		if (NEAR_ZERO(_velocity[0])) {
			_velocity[0] = 0;
			if (_colide) {
				delta_time -= correction_delta_time;
			}
		}

		float x = _velocity[0] * delta_time;
		float y = _velocity[1] * delta_time;
		if (abs(_velocity[1]) < 0.001) {
			y = 0;
			_velocity[1] = 0.001;
			if (_colide) {
				delta_time -= 0.01;
			}
		}
		_drawable->add(std::array<float, 2>{x, y});
	}

	auto drawable() const { return _drawable; }

   private:
	std::shared_ptr<PDrawable> _drawable;
	float _gravity;
	std::array<float, 2> _velocity;
	float _velocity_direction;
	float _restition;
	std::shared_ptr<PColider> _colider;
	bool _colide;
	std::array<float, 2> _colide_point;
	float _acceleration;
	std::shared_ptr<PPhysicObject> _colide_with;
	std::shared_ptr<PColision> _colision;
};

#endif	// PPHYSIC_H
