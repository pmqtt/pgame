#include "pphysic.h"


auto PPhysicObject::collide(std::shared_ptr<PPhysicObject> other, float time, float t) -> bool {
		if (_collider) {
			_collide = false;
			move(t * time);
			_collide = true;
			PPoint2D point{_drawable->x(), _drawable->y()};
			_colision = std::make_shared<PColision>(point, velocity_direction(), other, _collider->normals());
			return true;
		}
		return false;
	}

	auto PPhysicObject::are_colliding(std::shared_ptr<PPhysicObject> other, float t, float time) -> bool {
		std::shared_ptr<PPhysicObject> drawable = std::make_shared<PPhysicObject>(*this);
		drawable->_collide = false;
		drawable->move(t * time);
		if (_collider->collide(drawable->_drawable, other->_drawable)) {
			return true;
		}
		return false;
	}

	auto PPhysicObject::compute_toi(std::shared_ptr<PPhysicObject> other, float time) -> float {
		if (_collider) {
			float low = 0;
			float high = 1;	 // Assuming we are checking for TOI within the next frame, which is normalized to [0, 1]
			bool exist_colision = false;
			while (high - low > EPSILON) {
				float mid = (low + high) / 2.0;
				if (are_colliding(other, mid, time)) {
					exist_colision = true;
					high = mid;
				} else {
					low = mid;
				}
			}
			if (!exist_colision) {
				return -1.0;
			}
			float res = (low + high) / 2.0;
			return res;
		}
		return -1.0;
	}

	void PPhysicObject::move(float delta_time) {
		if (NEAR_ZERO(_velocity[0]) && NEAR_ZERO(_velocity[1])) {
			return;
		}

		if (_collide) {
			auto collision_normal = _colision->normals.normalized();
			if(collision_normal[0] != collision_normal[0] || collision_normal[1] != collision_normal[1]){
				return;
			}
			const float dot_product = _velocity[0] * collision_normal[0] + _velocity[1] * collision_normal[1];
			_velocity[0] -= (1 + _restition) * dot_product * collision_normal[0];
			_velocity[1] -= (1 + _restition) * dot_product * collision_normal[1];
		} else {
			_velocity[1] += _gravity * delta_time;
			_velocity[0] += _acceleration * delta_time;
		}


		float x = _velocity[0] * delta_time;
		float y = _velocity[1] * delta_time;
		
		if (NEAR_ZERO(_velocity[0])) {
			x = 0;
			_velocity[0] = 0;
		}
		
		if (NEAR_ZERO(_velocity[1])) {
			y = 0;
			_velocity[1] = 0.00001;
		}
		_drawable->add(std::array<float, 2>{x, y});
	}