#include "RubberBandForceGenerator.h"
#include "Particle.h"
using namespace physx;
RubberBandForceGenerator::RubberBandForceGenerator(double k, double rl, Particle* p):
	SpringForceGenerator(k,rl,p)
{
}

PxVec3 RubberBandForceGenerator::calculate_force(Particle* p, double dt)
{
	PxVec3 force(0.0);
	PxVec3 relative_pos = other->getPosition() - p->getPosition();

	const float length = relative_pos.normalize();
	const float delta_x = length - resisting_length;
	
	//solo si la distancia entre las 2 particulas supera a la longitud de reposo ejerzo la fuerza
	if (delta_x>0) force = relative_pos * delta_x * K;
	return force;
}
