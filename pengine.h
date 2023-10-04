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
#include <chrono>
#include <algorithm>

#include "pquadtree.h"
#include "pphysic.h"

struct PTimeToInpact {
	PTimeToInpact(const std::string& name, const std::string & name2, float time, std::shared_ptr<PPhysicObject> object1,
				  std::shared_ptr<PPhysicObject> object2)
		: name(name), name2(name2), time(time), object1(object1), object2(object2) {}
	PTimeToInpact() = default;
	std::string name;
    std::string name2;
	float time;
	std::shared_ptr<PPhysicObject> object1;
	std::shared_ptr<PPhysicObject> object2;
};

struct PCollisionItem{
    PCollisionItem(const std::string& name1, const std::string& name2) : name1(name1), name2(name2) {}
    std::string name1;
    std::string name2;

    auto operator==(const PCollisionItem& other) const -> bool {
        return (name1 == other.name1 && name2 == other.name2) || (name1 == other.name2 && name2 == other.name1);
    }

    friend auto operator<<(std::ostream& os, const PCollisionItem& item) -> std::ostream& {
        os << item.name1 << " " << item.name2;
        return os;
    }

};

namespace std {
    template<>
    struct hash<PCollisionItem> {
        auto operator()(const PCollisionItem& k) const -> std::size_t {
            return std::hash<std::string>()(k.name1) + (std::hash<std::string>()(k.name2));
        }
    };
}

class PEngine {
   public:
	PEngine() = default;
	~PEngine() = default;

	void add_physic_object(const std::string& name, std::shared_ptr<PPhysicObject> physic) {
		physic->name(name);
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

    auto collisions() -> std::vector<PCollisionItem> {
        return _collisions;
    }

	void handle_collision(double delta_time) {
		std::size_t elements_count = _physic_objects.size();
        //Store Items to handle collision events
        _collisions.clear();
		
		PQuadtree<std::shared_ptr<PPhysicObject>> quadtree(0, 0, 1200, 800);

		// Insert all physics objects into the quadtree
		for (auto& iter : _physic_objects) {
			quadtree.insert(iter.second);
		}

		std::vector<std::pair<std::shared_ptr<PPhysicObject>, std::shared_ptr<PPhysicObject>>> last_collisions;
		auto is_in_last_collisions = [&last_collisions](std::shared_ptr<PPhysicObject> object1,
													   std::shared_ptr<PPhysicObject> object2) {
			for (auto& iter : last_collisions) {
				if ((iter.first == object1 && iter.second == object2) || (iter.first == object2 && iter.second == object1)) {
					return true;
				}
			}
			return false;
		};

        std::vector<PTimeToInpact> collisions;
		PTimeToInpact last_collision;
		_collison_names.clear();
        std::size_t max_iteartions = elements_count *2;
		while(max_iteartions > 0 ) {
			collisions.clear();
			for (auto& iter : _physic_objects) {
				std::vector<std::shared_ptr<PPhysicObject>> objects = quadtree.retrieve(iter.second);
				for (auto& sIter : objects) {
					if (iter.first != sIter->name() && (iter.second->velocity().quad_length() > 0.0 || sIter->velocity().quad_length() > 0.0 )) {
						float t = iter.second->compute_toi(sIter, delta_time);
						if (t >= 0.0 && last_collision.object1 != iter.second && last_collision.object2 != sIter) {
							collisions.emplace_back(iter.first,sIter->name(), t, iter.second, sIter);
						} else {
							iter.second->reset_collision();
						}
					} else {
						iter.second->reset_collision();
					}
				}
			}
            if(collisions.size() == 0) {
                return;
            }
			std::sort(collisions.begin(), collisions.end(), [](auto& a, auto& b) { return a.time < b.time; });
			for (std::size_t i = 0; i < collisions.size(); ++i) {
				collisions[i].object1->colide(collisions[i].object2, delta_time, collisions[i].time);
				collisions[i].object2->colide(collisions[i].object1, delta_time, collisions[i].time);
				collisions[i].object2->move(delta_time- collisions[i].time * delta_time);
				collisions[i].object1->move(delta_time- collisions[i].time * delta_time);
				last_collision = collisions[i];
				_collison_names.insert(collisions[i].name);
                _collisions.push_back(PCollisionItem(collisions[i].name, collisions[i].name2));
				quadtree.update(collisions[i].object1);
				quadtree.update(collisions[i].object2);
			}
			max_iteartions--;
		}
	}

   private:
	std::map<std::string, std::shared_ptr<PPhysicObject>> _physic_objects;
	std::set<std::string> _collison_names;
    std::vector<PCollisionItem> _collisions;
    
};

#endif	// PENGINE_H
