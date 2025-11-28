#include "SpringForceGenerator.h"
#include "Particle.h"
#include <algorithm>
#include <iostream>
using namespace physx;
SpringForceGenerator::SpringForceGenerator(double k, double rl, Particle* p):ForceGenerator(), K(k), resisting_length(rl), other(p), umbral(0.5)
{
}

void SpringForceGenerator::setK(double newK)
{
	K = std::max<double>(newK, 0.0); 
}

void SpringForceGenerator::handle_special_input(int key)
{
	double newK = K;
	switch (key)
	{
	case GLUT_KEY_UP:
		newK += umbral;
		break;
	case GLUT_KEY_DOWN:
		newK -= umbral;
		break;
	}
	setK(newK);
	std::cout << "nuevo umbral es: " << newK << "\n";
}

void SpringForceGenerator::handle_input(unsigned char key)
{
	switch (toupper(key)) {
	case 'F':
		extra_force_active = true;
		break;
	}
}

void SpringForceGenerator::update_force(Particle* p, double dt)
{
	ForceGenerator::update_force(p, dt);
	if (extra_force_active)
	{
		p->add_force(extra_force_value);
		extra_force_active = false;
	}
}

physx::PxVec3 SpringForceGenerator::calculate_force(Particle* p, double dt)
{
	PxVec3 relative_pos = other->getPosition() - p->getPosition();

	const float length = relative_pos.normalize();
	const float delta_x = length - resisting_length;

	PxVec3 force = relative_pos * delta_x * K;
	return force;
}
