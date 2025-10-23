#include "Projectile.h"
using namespace physx;
#include "Projectile.h"

Projectile::Projectile(
    const PxVec3& p,
    const PxVec4& c,
    const PxVec3& v,
    const PxVec3& a,
    float m,
    double lt,
    physx::PxShape* sh
) : Particle(p, c, v, a, VERLET, m, lt, sh) 
{
}

Projectile::~Projectile() {}