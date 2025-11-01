#pragma once
#include "ForceGenerator.h"
#include "Particle.h"
class GravityForceGenerator : public ForceGenerator
{
public:
    GravityForceGenerator(const physx::PxVec3& g);
    virtual ~GravityForceGenerator() = default;

    void update_force(Particle* p) override;
protected:
    Vector3 gravity;
    physx::PxVec3 calculate_force(Particle* p) override;
};