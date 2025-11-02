#pragma once
#include "Particle.h"
class Projectile :
    public Particle
{
public: 
    Projectile(const physx::PxVec3& p,
        const physx::PxVec4& c,
        const physx::PxVec3& v,
        float m,
        double lt, 
        physx::PxShape* g,
        double vol);
    virtual ~Projectile();
};

