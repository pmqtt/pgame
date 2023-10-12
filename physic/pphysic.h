#ifndef PPHYSIC_H
#define PPHYSIC_H
#include <SDL2/SDL.h>

#include <thread>
#include <tuple>


#include "pcollider.h"
#include "pforce.h"
#include "../core/pmath.h"
#include "../core/pprimitive.h"
#include "../core/ptimer.h"


class PPhysicObject;

class PPhysicObject : public std::enable_shared_from_this<PPhysicObject> {
   public:
	PPhysicObject(std::shared_ptr<PDrawable> drawable)
		: std::enable_shared_from_this<PPhysicObject>(),
          _drawable(drawable),
		  _collider(nullptr),
		  _acceleration(0),
		   _restition(1),
		  _velocity(0, 0){
	}

	PPhysicObject(std::shared_ptr<PDrawable> drawable, std::shared_ptr<PCollider> collider)
		: std::enable_shared_from_this<PPhysicObject>(),
          _drawable(drawable),
		  _collider(collider),
		  _acceleration(0),
		  _restition(1),
		  _velocity(0, 0){
	}

	PPhysicObject(const PPhysicObject& a) : std::enable_shared_from_this<PPhysicObject>(){
		_name = a._name;
		_drawable = a._drawable->clone();
		_velocity = a._velocity;
		_restition = a._restition;
		_acceleration = a._acceleration;
		_collider = a._collider;
        _mass = a._mass;
	}

	virtual ~PPhysicObject() = default;


	// Set the velocity for this object in pixels per second
	void velocity(float x, float y) {
		_velocity.x(x);
		_velocity.y(y);
	}

	void velocity(const PPoint2D& v) { _velocity = v; }

	auto velocity() const -> PPoint2D { 
		if(_is_static){
			return {0,0};
		}
		return _velocity; 
	}

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
	auto restitution() const -> float { return _restition; }

	auto are_colliding(std::shared_ptr<PPhysicObject> other, float t, float time) -> bool ;

	auto compute_toi(std::shared_ptr<PPhysicObject> other, float time) -> float ;

	void move(float delta_time);

	auto drawable() const ->std::shared_ptr<PDrawable> { return _drawable; }

	auto position() const -> PPoint2D { return {_drawable->x(), _drawable->y()}; }

	auto name() const -> std::string { return _name; }

	void name(const std::string & name){
		_name = name;
	}

	void add_force(std::shared_ptr<PForce> force) { _forces.push_back(force); }


	void update(float deltaTime) {
        if(_is_static){
			return;
		}
		for (const auto& force : _forces) {
            force->apply_to(shared_from_this(), deltaTime);
        }
		float x = _velocity[0] * deltaTime;
		float y = _velocity[1] * deltaTime;
		_drawable->add({x, y});
    }

	void mass(float mass) { _mass = mass; }	
	auto mass() const -> float { return _mass; }
	auto inv_mass() const->float{ 
		if(_mass == 0){
			return 0;
		}
		return 1.0 / _mass;
	}

	void rotation(float angle) { _rotation[0] = angle; }
	void rotation_velocity(float vel){ _rotation[1] = vel; }
	void rotation_vec(const PPoint2D& vec){ _rotation = vec; }
	auto rotation() const -> const PPoint2D & { return _rotation; }

	auto is_static() const -> bool { return _is_static; }
	void is_static(bool value) { _is_static = value; }
	
	auto collider() const -> std::shared_ptr<PCollider> { return _collider; }

	void round_velocity_to_zero(){
		if(NEAR_ZERO(_velocity[0])){
			_velocity[0] = 0;
		}
		if(NEAR_ZERO(_velocity[1])){
			_velocity[1] = 0;
		}
	}

   private:
    std::string _name;
	std::shared_ptr<PDrawable> _drawable;
	std::shared_ptr<PCollider> _collider;
	std::vector<std::shared_ptr<PForce>> _forces;

	float _acceleration;
	float _restition;
	float _mass = 0.0;

	PPoint2D _velocity;
	PPoint2D _rotation; // x = angle, y = angular velocity
	bool _is_static = true;
};

template<class... Args>
auto make_physic_object(Args&&... args) -> std::shared_ptr<PPhysicObject> {
	return std::make_shared<PPhysicObject>(std::forward<Args>(args)...);
}

#endif	// PPHYSIC_H
