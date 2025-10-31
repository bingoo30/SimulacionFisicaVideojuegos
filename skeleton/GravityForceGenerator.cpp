#include "GravityForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(const physx::PxVec3& g): ForceGenerator(), gravity(g)
{
}

void GravityForceGenerator::update_force(Particle* p)
{

    if (p != nullptr) {
        if (p->getMass() <= 0.0f) return;
        p->add_force(gravity * p->getMass());
    }
}
