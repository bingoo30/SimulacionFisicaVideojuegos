#include "WindForceGenerator.h"
#include "Particle.h"
#include "StructForEntities.h"
#include <algorithm>
using namespace physx;
using namespace std;
WindForceGenerator::WindForceGenerator(const physx::PxVec3& pos, const physx::PxVec3& v, const physx::PxVec3& wa, double _k1, double d):ForceGenerator(), vel(v), area(wa),k1(_k1),k2(0.0),density(1.225), dragCoef(d), center(pos)
{
}

PxVec3 WindForceGenerator::calculate_force(Particle* p)
{
    if (!p) return PxVec3(0.0f);

    // si la masa es infinita (invMass ~ 0) => no aplicar
    if (abs(1.0 / p->getMass()) < 1e-10) return PxVec3(0.0f);

    // Comprobar si está dentro del área de influencia del viento
    PxVec3 pos = p->getPosition();
    PxVec3 relPos = pos - center;

    if (abs(relPos.x) > area.x * 0.5 ||
        abs(relPos.y) > area.y * 0.5 ||
        abs(relPos.z) > area.z * 0.5)
    {
        // fuera del área: sin viento
        return PxVec3(0.0f);
    }

    //radio
    double r = p->getRadius();
    if (r <= 0.0) {
        // protección: si r == 0, devolvemos 0 para evitar nan/inf
        return PxVec3(0.0f);
    }

    // Área frontal proyectada para una esfera = PI * r^2
    double areaFrontal = PI * r * r;

    // k2 calculado localmente (no modificar el miembro k2)
    double k2_local = 0.5 * density * dragCoef * areaFrontal;
    // NOTA: la forma física más habitual del drag es 0.5 * rho * Cd * A * v^2.
    // Tú usas F = k1*v + k2*|v|*v, con k2_local = 0.5*rho*Cd*A

    physx::PxVec3 dv = vel - p->getVelocity();
    double vMag = dv.magnitude();
    if (vMag < 1e-8) return physx::PxVec3(0.0f);

    physx::PxVec3 windF = static_cast<float>(k1) * dv + static_cast<float>(k2_local * vMag) * dv;
    return windF;
}

void WindForceGenerator::update_force(Particle* p)
{
    if (!p) return;
    p->add_force(calculate_force(p));
}
