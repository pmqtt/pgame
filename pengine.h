#ifndef PENGINE_H
#define PENGINE_H
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>
#include <set>

#include "pphysic.h"

struct PTimeToInpact {
	PTimeToInpact(const std::string& name, float time, std::shared_ptr<PPhysicObject> object1,
				  std::shared_ptr<PPhysicObject> object2)
		: name(name), time(time), object1(object1), object2(object2) {}
	PTimeToInpact() = default;
	std::string name;
	float time;
	std::shared_ptr<PPhysicObject> object1;
	std::shared_ptr<PPhysicObject> object2;
};

class PEngine {
   public:
	PEngine() = default;
	~PEngine() = default;

	void add_physic_object(const std::string& name, std::shared_ptr<PPhysicObject> physic) {
		_physic_objects[name] = physic;
	}

	auto objects() -> std::map<std::string, std::shared_ptr<PPhysicObject>> {
		std::map<std::string, std::shared_ptr<PPhysicObject>> result;
		for (auto& iter : _physic_objects) {
			if (std::find(_collison_names.begin(), _collison_names.end(), iter.first) == _collison_names.end()) {
				result[iter.first] = iter.second;
			}
		}
		return result;
	}

	void handle_collision(double delta_time) {
		std::size_t elements_count = _physic_objects.size();
		std::vector<PTimeToInpact> collisions;
		PTimeToInpact last_collision;
		_collison_names.clear();
        std::size_t max_iteartions = 100 + elements_count * elements_count;
		do {
			collisions.clear();
			for (auto& iter : _physic_objects) {
				for (auto& sIter : _physic_objects) {
					if (iter.first != sIter.first) {
						float t = iter.second->compute_toi(sIter.second, delta_time);
						if (t >= 0.0 && last_collision.object1 != iter.second &&
							last_collision.object2 != sIter.second) {
							collisions.emplace_back(iter.first, t, iter.second, sIter.second);
						} else {
							iter.second->reset_collision();
						}
					} else {
						iter.second->reset_collision();
					}
				}
			}
			std::sort(collisions.begin(), collisions.end(), [](auto& a, auto& b) { return a.time < b.time; });
			for (std::size_t i = 0; i < collisions.size(); ++i) {
				collisions[i].object1->colide(collisions[i].object2, delta_time);
				collisions[i].object1->move(delta_time);
				last_collision = collisions[i];
				_collison_names.insert(collisions[i].name);
			}
		} while (collisions.size() > 0 || max_iteartions-- > 0);
	}

   private:
	std::map<std::string, std::shared_ptr<PPhysicObject>> _physic_objects;
	std::set<std::string> _collison_names;
};

#endif	// PENGINE_H
