#include "GroundSystem.h"
#include "UniformGeneratorRB.h"
#include <iostream>
using namespace physx;
extern PxPhysics* gPhysics;
GroundSystem::GroundSystem(const Ground_Data& pd, int n, const physx::PxFilterData& dt, const physx::PxVec3& md, bool _trigger):
	RigidBodySystem(pd, Particle_Deviation_Data(), n, PxGeometryType::eBOX, true, md, dt), trigger(_trigger)
{
}

void GroundSystem::init()
{
	spawn_acu = -1;
	auto g = new UniformGeneratorRB(true);
	g->setFilter(filter);
	g->setTrigger(trigger);
	add_generator(g);
}

void GroundSystem::setMaterial(const physx::PxVec3& md)
{
	mat = gPhysics->createMaterial(md.x, md.y, md.z);
}

void GroundSystem::spawn(bool _render, bool _isStatic)
{
	RigidBodySystem::spawn(_render, _isStatic);
}

bool GroundSystem::check_out_of_limit(Particle* p) const
{
	return false;
}
