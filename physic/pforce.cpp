#include "pphysic.h"
#include "pforce.h"

void PGravity::apply_to(std::shared_ptr<PPhysicObject> obj, float deltaT) {
    const PVector2D v = obj->velocity();
    if(v.quad_length() > 0){
        const PVector2D v_new = v + _gravity * deltaT;
        obj->velocity(v_new);
    }
}
