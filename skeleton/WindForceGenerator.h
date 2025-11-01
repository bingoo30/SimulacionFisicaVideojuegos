#pragma once
#include "ForceGenerator.h"
class WindForceGenerator :
    public ForceGenerator 
{
public:
    WindForceGenerator(const physx::PxVec3& v, double _k1, double _k2, double r);
    virtual ~WindForceGenerator() = default;
    void update_force(Particle* p) override;
protected:
    Vector3 vel;
    double k1;
    double k2;
    double radius;

    physx::PxVec3 calculate_force(Particle* p) override;
};

