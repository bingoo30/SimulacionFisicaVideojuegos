#pragma once
#include "Particle.h"
class Projectile :
    public Particle
{
public: 
    Projectile(const physx::PxVec3& p,
        const physx::PxVec4& c,
        double m, 
        physx::PxShape* g,
        double vol,
        double lt,
        const physx::PxVec3& v);
    virtual ~Projectile() = default;
};

