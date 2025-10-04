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
) : Particle(p, c, v, a, d, VERLET, m, lt)
{

}

Projectile::~Projectile() {}

void Projectile::integrate(double t)
{

}
float calcular_masa_simulada(float mr, float vr, float vs) {
    return mr * pow(vr / vs, 2);
}
float calcular_gravedad_simulada(float gr, float vr, float vs) {
    return gr * pow(vr / vs, 2);
}