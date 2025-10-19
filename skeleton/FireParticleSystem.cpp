#include "FireParticleSystem.h"
#include "NormalGenerator.h"

FireParticleSystem::FireParticleSystem(const Particle_Data& pd, const Particle_Deviation_Data& pdd, int n): ParticleSystem(new NormalGenerator(), pd, pdd, n)
{
	spawnInterval = 0.05;
}
FireParticleSystem::~FireParticleSystem()
{
}
