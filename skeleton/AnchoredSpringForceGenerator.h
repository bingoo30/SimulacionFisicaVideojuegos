#pragma once
#include "SpringForceGenerator.h"
class Particle;
class AnchoredSpringForceGenerator : public SpringForceGenerator
{
public:
    AnchoredSpringForceGenerator(double k, double rl, const physx::PxVec3& pos);
    virtual ~AnchoredSpringForceGenerator() = default;
protected:
    physx::PxVec3 calculate_force(Particle* p, double dt) override;
    PxVec3 anchor;
};

