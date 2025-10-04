#pragma once
#include "Particle.h"
class Projectile :
    public Particle
{
public: 
    Projectile(const physx::PxVec3& p,
        const physx::PxVec4& c,
        const physx::PxVec3& v,
        const physx::PxVec3& a,
        double d,
        IntegrateMode t,
        float m,
        double lt);
    virtual ~Projectile();
};

