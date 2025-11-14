#pragma once
#include "ForceGenerator.h"

class SpringForceGenerator : public ForceGenerator
{
public:
    //(const physx::PxVec3& pos, const physx::PxVec3& v, const physx::PxVec3& wa, double _k1, double d)
    SpringForceGenerator(const physx::PxVec3& pos, double r, double k1, double d, double K, bool realistic);

    virtual ~SpringForceGenerator() = default;

    void update_force(Particle* p, double dt = 0.0) override;
protected:
    // constante proporcional a la fuerza del torbellino
    double k;
    double resisting_length;
    Particle* other;
};

