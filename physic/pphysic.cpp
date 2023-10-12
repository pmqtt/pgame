#include "pphysic.h"

auto PPhysicObject::are_colliding(std::shared_ptr<PPhysicObject> other, float t, float time) -> bool {
	std::shared_ptr<PPhysicObject> drawable = std::make_shared<PPhysicObject>(*this);
	drawable->update(t * time);
	if (_collider->collide(drawable->_drawable, other->_drawable)) {
		other->_collider->collide(other->_drawable, drawable->_drawable);
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
	
}
