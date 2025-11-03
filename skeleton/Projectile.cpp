#include "Projectile.h"
using namespace physx;
#include "Projectile.h"

Projectile::Projectile(const physx::PxVec3& p, const physx::PxVec4& c, double m, physx::PxShape* g, double vol, double lt, const physx::PxVec3& v)
:Particle(p, c, m, g, vol, lt, v, VERLET){}