#ifndef PFORCE_H
#define PFORCE_H
#include <memory>

class PPhysicObject;
struct PForce {
    virtual ~PForce() = default;
    virtual void apply_to(std::shared_ptr<PPhysicObject> obj, float deltaT) = 0;  
};

#endif // PFORCE_H