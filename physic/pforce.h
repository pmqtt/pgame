#ifndef PFORCE_H
#define PFORCE_H
#include <memory>

class PPhysicObject;
struct PForce {
    virtual ~PForce() = default;
    virtual void apply_to(std::shared_ptr<PPhysicObject> obj, float deltaT) = 0;  
};

class PGravity : public PForce {
public:
    PGravity(const PVector2D& gravity) : _gravity(gravity) {}

    void apply_to(std::shared_ptr<PPhysicObject> obj, float deltaT) override ;

private:
    PVector2D _gravity;
};

#endif // PFORCE_H