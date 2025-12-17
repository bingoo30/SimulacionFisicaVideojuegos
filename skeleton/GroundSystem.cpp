#include "GroundSystem.h"
#include "UniformGeneratorRB.h"
#include <iostream>
using namespace physx;
GroundSystem::GroundSystem(const Ground_Data& pd, int n, const physx::PxFilterData& dt, const physx::PxVec3& md):
	RigidBodySystem(pd, Particle_Deviation_Data(), n, PxGeometryType::eBOX, true, md, dt)
{
}

void GroundSystem::init()
{
	spawn_acu = -1;
	auto g = new UniformGeneratorRB(true);
	g->setFilter(filter);
	add_generator(g);
}

void GroundSystem::setMaterial(const physx::PxVec3& md)
{
	
}

void GroundSystem::spawn(bool _render, bool _isStatic)
{
	RigidBodySystem::spawn(_render, _isStatic);
}

bool GroundSystem::check_out_of_limit(Particle* p) const
{
	return false;
}
