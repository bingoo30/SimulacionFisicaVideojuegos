#pragma once
#include "ForceGenerator.h"
class Particle;
class BouyancyForceGenerator : public ForceGenerator
{
public:
    BouyancyForceGenerator(float h, float v, float d);
    virtual ~BouyancyForceGenerator();
    void register_aux_renders() override;
    void derregister_aux_renders() override;
protected:
    physx::PxVec3 calculate_force(Particle* p, double dt) override;
    float height;
    float volume;
    float liquid_density;
    //solo para la representacion visual
    Particle* liquid;
};
