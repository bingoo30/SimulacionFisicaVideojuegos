#include "RainParticleSystem.h"
#include "UniformGenerator.h"
RainParticleSystem::RainParticleSystem(const Particle_Data& pd, const Particle_Deviation_Data& pdd, int n):ParticleSystem(new UniformGenerator(), pd, pdd, n, physx::PxGeometryType::Enum::eCAPSULE)
{
	spawn_period = 0.1;
}

bool RainParticleSystem::check_out_of_limit(Particle* p) const
{
	float y = p->getPosition().y;
	return y < 0.0f;
}
