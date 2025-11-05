#include "Projectile.h"
Projectile::Projectile(const physx::PxVec3& p, const physx::PxVec4& c, float m, physx::PxShape* sh, double vol, double lt, const physx::PxVec3& v, IntegrateMode md, double de)
:Particle(p,c,m,sh,vol,lt,v,md, de)
{
}

Projectile::Projectile(const physx::PxVec3& p, const physx::PxVec4& c, float m, physx::PxShape* sh, double vol, double lt, const physx::PxVec3& v, IntegrateMode md, double de,double d)
:Particle(p, c, m, sh, vol, lt, v, md, d, de)
{
}

Projectile::~Projectile() {}