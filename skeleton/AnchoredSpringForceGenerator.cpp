#include "AnchoredSpringForceGenerator.h"
#include "Particle.h"
using namespace physx;
AnchoredSpringForceGenerator::AnchoredSpringForceGenerator(double k, double rl, const physx::PxVec3& pos):
	SpringForceGenerator(k,rl, nullptr), anchor(pos)
{
}

physx::PxVec3 AnchoredSpringForceGenerator::calculate_force(Particle* p, double dt)
{
    PxVec3 relative = anchor - p->getPosition();

    float length = relative.normalize();
    float delta = length - resisting_length;

    PxVec3 force = relative * delta * K;
    return force;
}
