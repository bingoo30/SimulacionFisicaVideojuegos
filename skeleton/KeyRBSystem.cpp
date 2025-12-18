#include "KeyRBSystem.h"
#include "UniformGeneratorRB.h"
using namespace physx;

KeyRBSystem::KeyRBSystem(const Particle_Data& pd, const physx::PxVec3& md, const physx::PxFilterData& dt): RigidBodySystem(pd, Particle_Deviation_Data(), 1, PxGeometryType::eCAPSULE, false, md, dt)
{
}

void KeyRBSystem::init()
{
	spawn_acu = -1;
	auto g = new UniformGeneratorRB(false);
	g->setFilter(filter);
	add_generator(g);
}

Particle* KeyRBSystem::getKey()
{
	if (particles_list.empty()) return nullptr;
	else return particles_list.front().get();
}

bool KeyRBSystem::check_out_of_limit(Particle* p) const
{
	return false;
}
