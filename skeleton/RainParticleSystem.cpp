#include "RainParticleSystem.h"
#include "UniformGenerator.h"
RainParticleSystem::RainParticleSystem(const Particle_Data& pd, const Particle_Deviation_Data& pdd, int n):ParticleSystem(pd, pdd, n, physx::PxGeometryType::Enum::eCAPSULE)
{
	add_generator(new UniformGenerator());
	spawn_acu = 0.1;
}

bool RainParticleSystem::check_out_of_limit(Particle* p) const
{
	if (!p) return true;
	float y = p->getPosition().y;
	return y < 0.0f;
}
