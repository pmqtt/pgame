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

static int id = 0;

struct PColision {
	PColision(const PPoint2D& point, float direction_angle, std::shared_ptr<PPhysicObject> colide_with,
			  const PPoint2D& normals)
		: point(point), direction_angle(direction_angle), colide_with(colide_with), normals(normals), handled(false) {
		name = "colision_" + std::to_string(id++);
	}
	PPoint2D point;
	float direction_angle;
	std::shared_ptr<PPhysicObject> colide_with;
	PPoint2D normals;
	bool handled;
	std::string name;
};

class PPhysicObject : public std::enable_shared_from_this<PPhysicObject> {
   public:
	PPhysicObject(std::shared_ptr<PDrawable> drawable)
		: std::enable_shared_from_this<PPhysicObject>(),
          _drawable(drawable),
		  _gravity(0),
		  _velocity{0, 0},
		  _restition(1),
		  _acceleration(0),
		  _colision(nullptr) {
		_collide = false;
	}

	PPhysicObject(std::shared_ptr<PDrawable> drawable, std::shared_ptr<PCollider> colider)
		: std::enable_shared_from_this<PPhysicObject>(),
          _drawable(drawable),
		  _gravity(0.0),
		  _velocity{0, 0},
		  _restition(1),
		  _collider(colider),
		  _colision(nullptr) {
		_collide = false;
	}

	PPhysicObject(const PPhysicObject& a) : std::enable_shared_from_this<PPhysicObject>(){
		_name = a._name;
		_drawable = a._drawable->clone();
		_gravity = a._gravity;
		_velocity = a._velocity;
		_restition = a._restition;
		_acceleration = a._acceleration;
		_collider = a._collider;
		_colision = a._colision;
		_collide = a._collide;
		_timer = a._timer;
        _mass = a._mass;
	}

	virtual ~PPhysicObject() = default;

	void set_timer(const PTimer& timer) { _timer = timer; }

	// Set the gravity for this object in pixels per second
	void gravity(float gravity) { _gravity = gravity; }

	// Set the velocity for this object in pixels per second
	void velocity(float x, float y) {
		_velocity.x(x);
		_velocity.y(y);
	}

	void velocity(const PPoint2D& v) { _velocity = v; }

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

	auto collision_detected() const -> bool { return _collide; }

	auto reset_collision() -> void {
		_collide = false;
		_colision = nullptr;
	}

	auto collide(std::shared_ptr<PPhysicObject> other, float time, float t) -> bool ;

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

	void handle_collision(float deltaT);

	void update(float deltaTime) {
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
		return _mass > 0.0f ? 1.0f/_mass : 0.0f; 
	}

	void rotation(float angle) { _rotation[0] = angle; }
	void rotation_velocity(float vel){ _rotation[1] = vel; }
	void rotation_vec(const PPoint2D& vec){ _rotation = vec; }
	auto rotation() const -> const PPoint2D & { return _rotation; }

	auto is_static() const -> bool { return _is_static; }
	void is_static(bool value) { _is_static = value; }
	

   private:
    std::string _name;
	std::shared_ptr<PDrawable> _drawable;
	std::shared_ptr<PCollider> _collider;
	std::shared_ptr<PColision> _colision;
	std::vector<std::shared_ptr<PForce>> _forces;

	float _gravity;
	float _acceleration;
	float _restition;
	float _mass = 0.0;

	PPoint2D _velocity;

	PPoint2D _rotation; // x = angle, y = angular velocity
	
	bool _collide;
	PTimer _timer;
	
	bool _is_static = true;

};

#endif	// PPHYSIC_H
