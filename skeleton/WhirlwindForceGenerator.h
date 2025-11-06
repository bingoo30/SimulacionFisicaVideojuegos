#pragma once
#include "WindForceGenerator.h"

class WhirlwindForceGenerator : public WindForceGenerator
{
public:
    //(const physx::PxVec3& pos, const physx::PxVec3& v, const physx::PxVec3& wa, double _k1, double d)
    WhirlwindForceGenerator(const physx::PxVec3& pos, double r, double k1, double d, double K, bool realistic);

    virtual ~WhirlwindForceGenerator() = default;

    void update_force(Particle* p, double dt = 0.0) override;
protected:
    // constante proporcional a la fuerza del torbellino
    double k;
};

