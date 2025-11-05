#include "WhirlwindForceGenerator.h"
#include "Particle.h"
void WhirlwindForceGenerator::update_force(Particle* p)
{
    if (p == nullptr) return;

    const physx::PxVec3 r = p->getPosition() - center;
    vel = k * physx::PxVec3(-r.z, 50 - r.y, r.x);

    WindForceGenerator::update_force(p);
}

WhirlwindForceGenerator::WhirlwindForceGenerator(const physx::PxVec3& pos, double r, double k1, double d, double K, bool realistic):
WindForceGenerator(pos, physx::PxVec3(0.0), r, k1, d, realistic), k(K)
{
}
