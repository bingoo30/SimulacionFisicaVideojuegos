#include "WhirlwindForceGenerator.h"
#include "Particle.h"
void WhirlwindForceGenerator::update_force(Particle* p)
{
    if (p == nullptr) return;

    physx::PxVec3 r = p->getPosition() - center;
    vel = k * Vector3(-r.y, r.x, 0);

   // WindGenerator::updateForce(particle, dt);
}

WhirlwindForceGenerator::WhirlwindForceGenerator(const physx::PxVec3& pos, const physx::PxVec3& wa, double k1, double d, double r, double K)
{
    center=
}
