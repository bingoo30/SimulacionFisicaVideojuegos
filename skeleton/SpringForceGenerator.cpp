#include "SpringForceGenerator.h"
#include "Particle.h"
using namespace physx;
SpringForceGenerator::SpringForceGenerator(double k, double rl, Particle* p):ForceGenerator(), K(k), resisting_length(rl), other(p)
{
}

void SpringForceGenerator::setK(double newK)
{
	K = newK;
}

void SpringForceGenerator::update_force(Particle* p, double dt)
{
	p->add_force(calculate_force(p, dt));
}

physx::PxVec3 SpringForceGenerator::calculate_force(Particle* p, double dt)
{
	PxVec3 relative_pos = other->getPosition() - p->getPosition();

	const float length = relative_pos.normalize();
	const float delta_x = length - resisting_length;

	PxVec3 force = relative_pos * delta_x * K;
	return force;
}
