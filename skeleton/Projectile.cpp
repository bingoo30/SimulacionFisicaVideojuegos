#include "Projectile.h"
using namespace physx;
#include "Projectile.h"

Projectile::Projectile(
    const PxVec3& p,
    const PxVec4& c,
    const PxVec3& v,
    const PxVec3& a,
    double d,
    IntegrateMode t,
    float m,
    double lt
) : Particle(p, c, v, a, d, t, m, lt)
{

}

Projectile::~Projectile() {}