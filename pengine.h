#ifndef PENGINE_H
#define PENGINE_H
#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>
#include "pphysic.h"


struct PTimeToInpact{
    PTimeToInpact(float time, std::shared_ptr<PPhysicObject> object1, std::shared_ptr<PPhysicObject> object2)
        : time(time), object1(object1),object2(object2) {}
    float time;
    std::shared_ptr<PPhysicObject> object1;
    std::shared_ptr<PPhysicObject> object2;
};

class PEngine{
public:
    PEngine() = default;
    ~PEngine() = default;

    void add_physic_object(const std::string& name, std::shared_ptr<PPhysicObject> physic){
        _physic_objects[name] = physic;
    }

    auto objects() -> std::map<std::string, std::shared_ptr<PPhysicObject>> &{
        return _physic_objects;
    }

    void handle_collision(double delta_time){
        std::size_t elements_count = _physic_objects.size();
        std::vector<PTimeToInpact> collisions;
		for (auto& iter : _physic_objects) {
			for (auto& sIter : _physic_objects) {
				if (iter.first != sIter.first) {
					float t = iter.second->compute_toi(sIter.second, delta_time);
                    if(t >= 0.0){
                        std::cout<<"collision "<< iter.first<< " "<< sIter.first<< " "<< t<<std::endl;
                        collisions.emplace_back(t, iter.second, sIter.second);
                    }else{
                        iter.second->reset_collision();
                    }
				}
			}
		}
        std::sort(collisions.begin(), collisions.end(), [](auto& a, auto& b){
            return a.time < b.time;
        });
        for(auto& iter : collisions){
            iter.object1->colide(iter.object2,delta_time);
        }
		
    }

private:
	std::map<std::string, std::shared_ptr<PPhysicObject>> _physic_objects;
};

#endif //PENGINE_H
