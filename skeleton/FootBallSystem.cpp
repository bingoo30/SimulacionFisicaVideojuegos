#include "FootBallSystem.h"
#include "NormalGeneratorRB.h"
using namespace physx;
FootBallSystem::FootBallSystem(const Football_Data& pd, const Football_Deviation_Data& pdd, int n, const physx::PxVec3& md):
	RigidBodySystem(pd, pdd, n, PxGeometryType::eSPHERE, false, md, PxFilterData(0, 0, 0, 0))
{
}

void FootBallSystem::init()
{
	spawn_acu = -1;
	auto g = new NormalGeneratorRB(false);
	add_generator(g);
}

bool FootBallSystem::check_out_of_limit(Particle* p) const
{
	//auto rb = static_cast<DynamicRigidBody*>(p);

	//return rb->getPosition().y < 0;
	return false;
}
