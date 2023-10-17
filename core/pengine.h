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

#include "../datastructs/pquadtree.h"
#include "../physic/pphysic.h"
#include "../core/pmoveable.h"

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

	void add_physic_object(const std::string& name, const std::shared_ptr<PPhysicObject> & physic) {
		physic->name(name);
		_physic_objects[name] = physic;
	}

	void add_moveable(const std::string &name, const std::shared_ptr<PMoveable> &moveable) {
		_moveable_objects[name] = moveable;
	}

	auto all_objects() -> std::map<std::string, std::shared_ptr<PPhysicObject>> &{
		return _physic_objects;
	}

    auto collisions() -> std::vector<PCollisionItem> {
        return _collisions;
	}

	void simulate_step(double deltaT){
		std::size_t max_iter = 2*_physic_objects.size();
        _collisions.clear();
		PQuadtree<std::shared_ptr<PPhysicObject>> quadtree(0, 0, 1600, 600);
		for (auto& iter : _physic_objects) {
			quadtree.insert(iter.second);
		}
		float remainingTime = deltaT;
		std::list<std::pair<std::shared_ptr<PPhysicObject>, std::shared_ptr<PPhysicObject>>> last_collisions;
    	while (remainingTime > 0 && max_iter-- > 0) {
			float earliestTOI = remainingTime;
			std::shared_ptr<PPhysicObject> objA;
			std::shared_ptr<PPhysicObject> objB;
			for (auto & obj : _physic_objects) {
				if(obj.second->is_static()){
					continue;
				}
				auto candidates = quadtree.retrieve(obj.second);
				bool should_break = false;
				for (auto & candidate : candidates) {
					if (obj.second != candidate) {
						if(!last_collisions.empty()){
							for(auto & last_collision : last_collisions){
								if(last_collision.first == obj.second && last_collision.second == candidate){
									should_break = true;
								}
								if(last_collision.first == candidate && last_collision.second == obj.second){
									should_break = true;
								}
							}
						}
                        if(should_break){
							continue;
						} 
						if(obj.second->is_static() && candidate->is_static()){
							continue;
						}
                        if(NEAR_ZERO(obj.second->velocity().quad_length()) && NEAR_ZERO(candidate->velocity().quad_length())){
							continue;
						}
						float toi;
						toi = obj.second->compute_toi(candidate, remainingTime);
                        if (toi > -1 && toi < earliestTOI) {
								earliestTOI = toi;
								objA = obj.second;
								objB = candidate;
						}
					}
				}
			}
			if (objA && objB) {
			  	for (auto & obj : _physic_objects) {
        			obj.second->update(earliestTOI);
    			}
				handle_collision(objA, objB, earliestTOI);
				last_collisions.emplace_back(objA, objB);
				remainingTime -= earliestTOI;
				quadtree.update(objA);
				quadtree.update(objB);
				_collisions.emplace_back(objA->name(), objB->name());
			} else {
				for (auto & obj : _physic_objects) {
        			obj.second->update(earliestTOI);
    			}
				remainingTime = 0;
			}
		}
	}


	static auto handle_collision( std::shared_ptr<PPhysicObject> &a, std::shared_ptr<PPhysicObject> &b, float delta_time) -> bool {
		P_UNUSED(delta_time);
		if(0){
			std::cout<<"\t\t\t\tHandle collisions: "<<a<<"\n";
			std::cout<<"\t\t\t\t                   "<<b<<"\n";
		}
		const auto v1 = a->velocity();
		const auto v2 = b->velocity(); 
		const auto velocity = v1 - v2;
	
		if (NEAR_ZERO(velocity[0]) && NEAR_ZERO(velocity[1])) {
			return false;
		}
		//const auto collider = a->collider();


		auto collision_normal =  a->collider()->normals().normalized() * -1.0f;
		if(collision_normal[0] != collision_normal[0] || collision_normal[1] != collision_normal[1]){
            return false;
		}

		if(velocity.dot(collision_normal) > 0){
			return false;
		}
		a->drawable()->add(a->collider()->mtv()* -1.0f);
		const float restitation = std::min(a->restitution(), b->restitution());
		const float dot_product = velocity[0] * collision_normal[0] + velocity[1] * collision_normal[1];
	
		float J = -(1.0f + restitation) * dot_product;
        if(a->inv_mass() + b->inv_mass() != 0){
			J /= a->inv_mass() + b->inv_mass();
			PVector2D impulse =  collision_normal * J;
			a->velocity(v1 + impulse * a->inv_mass());
			b->velocity(v2 - impulse * b->inv_mass());
		}else{
			PVector2D impulse =  collision_normal * J;
			a->velocity(v1 + impulse );
			b->velocity(v2 - impulse );
		}
		//std::cout<<"J: "<<J<< " v:"<<a->velocity()<<"\n";
		a->round_velocity_to_zero();
		b->round_velocity_to_zero();
		return true;
	}

   private:
	std::map<std::string, std::shared_ptr<PPhysicObject>> _physic_objects;
	std::map<std::string, std::shared_ptr<PMoveable>> _moveable_objects;
    std::vector<PCollisionItem> _collisions;

};

#endif	// PENGINE_H
