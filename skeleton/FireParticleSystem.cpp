#include "FireParticleSystem.h"
#include "NormalGenerator.h"
#include <algorithm>

FireParticleSystem::FireParticleSystem(const Particle_Data& pd, const Particle_Deviation_Data& pdd, int n) : ParticleSystem(new NormalGenerator(), pd, pdd, n, physx::PxGeometryType::Enum::eSPHERE)
{
	spawn_period = 0.05;
}

bool FireParticleSystem::check_out_of_limit(Particle* p) const
{
	const physx::PxVec3& pos = p->getPosition();
	return pos.y < 0.0 || std::abs(pos.x) > deviation.valid_box || std::abs(pos.z) > deviation.valid_box;
}
