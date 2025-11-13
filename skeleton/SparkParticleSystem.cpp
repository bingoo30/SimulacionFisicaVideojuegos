#include "SparkParticleSystem.h"
#include "Particle.h"
#include "UniformGenerator.h"
#include <cmath>
using namespace physx;

SparkParticleSystem::SparkParticleSystem(const Particle_Data& pd, const Particle_Deviation_Data& pdd, int n) :
	ParticleSystem(pd, pdd, n, PxGeometryType::Enum::eSPHERE)
{
	add_generator(new UniformGenerator());
}

bool SparkParticleSystem::check_out_of_limit(Particle* p) const
{
	return false;
}
