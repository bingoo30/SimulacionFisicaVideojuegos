#include "RubberBandForceGenerator.h"
using namespace physx;
RubberBandForceGenerator::RubberBandForceGenerator(double k, double rl, double ml, Particle* p): SpringForceGenerator(k,rl,p), max_length(ml)
{
}

void RubberBandForceGenerator::update_force(Particle* p, double dt)
{
}

PxVec3 RubberBandForceGenerator::calculate_force(Particle* p, double dt)
{
	return physx::PxVec3();
}
