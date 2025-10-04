#include "Projectile.h"
using namespace physx;
#include "Projectile.h"

Projectile::Projectile(
    const PxVec3& p,
    const PxVec4& c,
    const PxVec3& v,
    const PxVec3& a,
    double d,
    float m,
    double lt
) : Particle(p, c, v, a, d, EULER_SEMIIMPLICIT, m, lt)
{
}

Projectile::~Projectile() {}