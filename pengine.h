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
		
        std::vector<PTimeToInpact> collisions;
		PTimeToInpact last_collision;
		_collison_names.clear();
        std::size_t max_iteartions = 100 + elements_count * elements_count;
		do {
			collisions.clear();
            //measure execution time
			for (auto& iter : _physic_objects) {
				for (auto& sIter : _physic_objects) {
					if (iter.first != sIter.first && (iter.second->velocity().quad_length() > 0.0 || sIter.second->velocity().quad_length() > 0.0 )) {
						float t = iter.second->compute_toi(sIter.second, delta_time);
						if (t >= 0.0 && last_collision.object1 != iter.second &&
							last_collision.object2 != sIter.second) {
							collisions.emplace_back(iter.first,sIter.first, t, iter.second, sIter.second);
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
				collisions[i].object1->move(delta_time);
				last_collision = collisions[i];
				_collison_names.insert(collisions[i].name);
                _collisions.push_back(PCollisionItem(collisions[i].name, collisions[i].name2));
			}
		} while (collisions.size() > 0 || max_iteartions-- > 0);
	}

   private:
	std::map<std::string, std::shared_ptr<PPhysicObject>> _physic_objects;
	std::set<std::string> _collison_names;
    std::vector<PCollisionItem> _collisions;
    
};

#endif	// PENGINE_H
