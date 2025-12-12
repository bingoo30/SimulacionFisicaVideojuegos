#include "GroundSystem.h"
#include "UniformGeneratorRB.h"
using namespace physx;
GroundSystem::GroundSystem(const Ground_Data& pd, int n, const physx::PxVec3& md): 
	RigidBodySystem(pd, Particle_Deviation_Data(), n, PxGeometryType::eBOX, true, md)
{

}

void GroundSystem::init()
{
	spawn_acu = -1;
	add_generator(new UniformGeneratorRB(true));
}

bool GroundSystem::check_out_of_limit(Particle* p) const
{
	return false;
}
