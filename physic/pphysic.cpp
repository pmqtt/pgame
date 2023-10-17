#include "pphysic.h"

auto PPhysicObject::are_colliding(const std::shared_ptr<PPhysicObject>& other, float t, float time) -> bool {
	std::shared_ptr<PPhysicObject> drawable = std::make_shared<PPhysicObject>(*this);
	drawable->update(t * time);
	if (_collider->collide(drawable->_drawable, other->_drawable)) {
		return true;
	}
	return false;
}

auto PPhysicObject::compute_toi(const std::shared_ptr<PPhysicObject>& other, float time) -> float {
	if (_collider) {
		if(are_colliding(other, 0.0f, time)){
			return 0.0f;
		}
		float low = 0;
		float high = 1;	 // Assuming we are checking for TOI within the next frame, which is normalized to [0, 1]
		bool exist_collision = false;
		while (high - low > EPSILON) {
			float mid = (low + high) / 2.0f;
			if (are_colliding(other, mid, time)) {
				exist_collision = true;
				high = mid;
			} else {
				low = mid;
			}
		}
		if (!exist_collision) {
			return -1.0;
		}
		float res = (low + high) / 2.0f;
		return res;
	}
	return -1.0;
}

void PPhysicObject::move(float delta_time) {
	P_UNUSED(delta_time);
}

auto operator<<(std::ostream& os, const std::shared_ptr<PPhysicObject>& obj) -> std::ostream&{
	std::string static_str = obj->is_static() ? "yes" : "no";
	os << "name:"     << obj->name()     << " " 
	   << "pos:"      << obj->position() << " "
	   << "velocity:" << obj->velocity() << " " 
	   << "rotation"  << obj->rotation() << " " 
	   << "mass:"     << obj->mass()     << " " 
	   << "static:"   << static_str;
	return os;
}
