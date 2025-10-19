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
        float m,
        double lt, 
        double vol);
    virtual ~Projectile();
};

