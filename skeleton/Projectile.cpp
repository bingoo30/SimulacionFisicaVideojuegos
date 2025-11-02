#include "Projectile.h"
using namespace physx;
#include "Projectile.h"

Projectile::Projectile(
    const PxVec3& p,
    const PxVec4& c,
    const PxVec3& v,
    float m,
    double lt,
    physx::PxShape* sh,
    double vol
) : Particle(p, c, v, VERLET, m, lt, sh, vol) 
{
}

Projectile::~Projectile() {}