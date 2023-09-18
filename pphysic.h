#ifndef PPHYSIC_H
#define PPHYSIC_H
#include <SDL2/SDL.h>
#include <tuple>
#include <thread>

#include "pcolider.h"
#include "pmath.h"
#include "pprimitive.h"
#include "ptimer.h"


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
		  _colision(nullptr){
		_colide = false;
	}

    PPhysicObject(const PPhysicObject& a){
        _drawable = a._drawable->clone();
        _gravity = a._gravity;
        _velocity = a._velocity;
        _velocity_direction = a._velocity_direction;
        _restition = a._restition;
        _acceleration = a._acceleration;
        _colider = a._colider;
        _colision = a._colision;
        _colide = a._colide;
        _timer = a._timer;
    }

	virtual ~PPhysicObject() = default;

    void set_timer(const PTimer& timer){
        _timer = timer;
    }

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

	auto colide(std::shared_ptr<PPhysicObject> other,float time) -> bool {
		if (_colider) {
            float t = compute_toi(other,time);
            if(t < 0.0){
                _colide = false;
                _colision = nullptr;
                return false;
            }
            _colide = false;
            move(t*time);
			_colide = true;
            auto point = std::array<float, 2>{_drawable->x(), _drawable->y()};
			_colision = std::make_shared<PColision>(point, velocity_direction(), other, _colider->normals());
			return true;
		}
		_colide = false;
		_colision = nullptr;
		return false;
	}

    auto are_colliding( std::shared_ptr<PPhysicObject> other, float t,float time) -> bool {
        std::shared_ptr<PPhysicObject> drawable = std::make_shared<PPhysicObject>( *this );
        drawable->_colide = false;
        drawable->move(t*time);
        if(_colider->colide(drawable->_drawable, other->_drawable)){
            return true;
        }
        return false;
    }

    auto compute_toi(std::shared_ptr<PPhysicObject> other,float time) ->float{
        if(_colider){
            float low = 0;
            float high = 1; // Assuming we are checking for TOI within the next frame, which is normalized to [0, 1]
            while (high - low > EPSILON) {
                float mid = (low + high) / 2.0;
                if (are_colliding(other,mid,time)) {
                    high = mid;
                } else {
                    low = mid;
                }
            }
            if( !are_colliding(other,1.0,time) ) {
                return -1.0; 
            }
            float res = (low + high) / 2.0;
            return res;
        }
        return -1.0;
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
			_velocity[1] = 0.00001;
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
    PTimer _timer;
};

#endif	// PPHYSIC_H
