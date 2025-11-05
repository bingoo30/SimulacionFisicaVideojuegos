#pragma once
#include "WindForceGenerator.h"

class WhirlwindForceGenerator : public WindForceGenerator
{
public:
    //(const physx::PxVec3& pos, const physx::PxVec3& v, const physx::PxVec3& wa, double _k1, double d)
    WhirlwindForceGenerator(const physx::PxVec3& pos, const physx::PxVec3& wa, double k1, double d, double r, double K);

    virtual ~WhirlwindForceGenerator() = default;

    void update_force(Particle* p) override;
protected:
    double k;
};

