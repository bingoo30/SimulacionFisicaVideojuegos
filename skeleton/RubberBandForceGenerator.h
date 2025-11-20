#pragma once
#include "SpringForceGenerator.h"
class Particle;
class RubberBandForceGenerator : public SpringForceGenerator
{
public:
    RubberBandForceGenerator(double k, double rl, double ml,Particle* p);

    virtual ~RubberBandForceGenerator() = default;
    void update_force(Particle* p, double dt) override;
protected:
    physx::PxVec3 calculate_force(Particle* p, double dt) override;
    double max_length;
};

