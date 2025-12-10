#include "FootBallSystem.h"
#include "NormalGeneratorRB.h"
using namespace physx;
FootBallSystem::FootBallSystem(const Football_Data& pd, const Football_Deviation_Data& pdd, int n, const physx::PxVec3& md):
	RigidBodySystem(pd, pdd, n, PxGeometryType::eSPHERE, false, md)
{
}

void FootBallSystem::init()
{
	spawn_acu = -1;
	add_generator(new NormalGeneratorRB(false));
}

bool FootBallSystem::check_out_of_limit(Particle* p) const
{
	return false;
}
