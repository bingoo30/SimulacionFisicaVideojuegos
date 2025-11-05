#include "WindForceGenerator.h"
#include "Particle.h"
#include "StructForEntities.h"
#include <algorithm>
using namespace physx;
using namespace std;
WindForceGenerator::WindForceGenerator(const physx::PxVec3& pos, const physx::PxVec3& v, double r, double _k1, double d, bool moreRealistic):ForceGenerator(), vel(v), radius(r),k1(_k1),k2(0.0),density(1.225), dragCoef(d), center(pos), realistic(moreRealistic)
{
}

PxVec3 WindForceGenerator::calculate_force(Particle* p)
{
    if (!p) return PxVec3(0.0f);

    // si la masa es infinita (invMass ~ 0) => no aplicar
    if (abs(1.0 / p->getMass()) < 1e-10) return PxVec3(0.0f);

    // Comprobar si está dentro del área de influencia del viento
    PxVec3 relPos = p->getPosition() - center;

    if (relPos.magnitude() > radius) return PxVec3(0.0f);
    
    //si es realista, hago con el volumen
    if (realistic) {
        //radio
        double r = p->getRadius();
        if (r <= 0.0) {
            // protección: si r == 0, devolvemos 0 para evitar nan/inf
            return PxVec3(0.0f);
        }

        // Área frontal proyectada para una esfera = PI * r^2
        double areaFrontal = PI * r * r;

        // k2 calculado localmente
        k2 = 0.5 * density * dragCoef * areaFrontal;
    }
    //sino, pongo uno simulado
    else {
        k2 = 0;
    }
    physx::PxVec3 dv = vel - p->getVelocity();
    double vMag = dv.magnitude();
    if (vMag < 1e-8) return physx::PxVec3(0.0f);

    physx::PxVec3 windF = k1 * dv + k2 * vMag* dv;
    return windF;
}

void WindForceGenerator::update_force(Particle* p)
{
    if (!p) return;
    p->add_force(calculate_force(p));
}
