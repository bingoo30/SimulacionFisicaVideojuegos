#include "SpringForceGenerator.h"
#include "Particle.h"
#include <algorithm>
using namespace physx;
SpringForceGenerator::SpringForceGenerator(double k, double rl, Particle* p):ForceGenerator(), K(k), resisting_length(rl), other(p), umbral(5)
{
}

void SpringForceGenerator::setK(double newK)
{
	K = std::min<double>(newK, 0.0); 
}

void SpringForceGenerator::handle_special_input(int key)
{
	double newK = K;
	switch (key)
	{
	case GLUT_KEY_UP:
		newK += umbral;
		break;
	case GLUT_KEY_RIGHT:
		newK -= umbral;
		break;
	}
	setK(newK);
}

physx::PxVec3 SpringForceGenerator::calculate_force(Particle* p, double dt)
{
	PxVec3 relative_pos = other->getPosition() - p->getPosition();

	const float length = relative_pos.normalize();
	const float delta_x = length - resisting_length;

	PxVec3 force = relative_pos * delta_x * K;
	return force;
}
