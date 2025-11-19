#include "GravityForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(const physx::PxVec3& g): ForceGenerator(), gravity(g)
{
}

physx::PxVec3 GravityForceGenerator::calculate_force(Particle* p, double dt)
{
    return gravity * p->getMass();
}
