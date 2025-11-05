#pragma once
#include "Particle.h"
class Projectile :
    public Particle
{
public: 
   /* Particle::Particle(const physx::PxVec3& p, const physx::PxVec4& c, float m, physx::PxShape* sh, double vol, double lt, const physx::PxVec3& v, IntegrateMode md, double d)
        :Entity(p, c, m, sh, vol, lt, md), last_pos(p), vel(v), damping(d)
    {
    }*/
    Projectile(const physx::PxVec3& p, const physx::PxVec4& c, float m, physx::PxShape* sh, double vol, double lt, const physx::PxVec3& v, IntegrateMode md, double de);
    Projectile(const physx::PxVec3& p, const physx::PxVec4& c, float m, physx::PxShape* sh, double vol, double lt, const physx::PxVec3& v, IntegrateMode md, double de, double d);
    virtual ~Projectile();
};

