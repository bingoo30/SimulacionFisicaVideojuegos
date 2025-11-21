#pragma once
#include "SpringForceGenerator.h"
class Particle;
class RubberBandForceGenerator : public SpringForceGenerator
{
public:
    RubberBandForceGenerator(double k, double rl, Particle* p);
    virtual ~RubberBandForceGenerator() = default;
protected:
    physx::PxVec3 calculate_force(Particle* p, double dt) override;
};

