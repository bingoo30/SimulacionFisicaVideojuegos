#include "FireParticleSystem.h"
#include "NormalGenerator.h"
#include <algorithm>

FireParticleSystem::FireParticleSystem(const Particle_Data& pd, const Particle_Deviation_Data& pdd, int n) : ParticleSystem(pd, pdd, n, physx::PxGeometryType::Enum::eSPHERE)
{
	add_generator(new NormalGenerator());
	spawn_acu = 0.05;
}

bool FireParticleSystem::check_out_of_limit(Particle* p) const
{
	if (!p) return true;

	const physx::PxVec3& pos = p->getPosition();
	return pos.y <= 0.0 ||pos.y > model.pos.y + deviation.valid_box.y 
		|| std::abs(pos.x) > model.pos.x+deviation.valid_box.x
		|| std::abs(pos.z) > model.pos.z+deviation.valid_box.z;
}
