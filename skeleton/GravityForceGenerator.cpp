#include "GravityForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(const physx::PxVec3& g): ForceGenerator(), gravity(g)
{
}

void GravityForceGenerator::update_force(Particle* p, double dt)
{

    if (p != nullptr) {
        if (p->getMass() <= 0.0f) return;
        p->add_force(calculate_force(p));
    }
}

physx::PxVec3 GravityForceGenerator::calculate_force(Particle* p, double dt)
{
    return gravity * p->getMass();
}
