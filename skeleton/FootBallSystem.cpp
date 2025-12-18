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
	auto rb = static_cast<DynamicRigidBody*>(p);

	const physx::PxVec3& pos = rb->getPosition();
	return pos.y <= 0.0 || pos.y > model.pos.y + deviation.valid_box.y
		|| std::abs(pos.x) > model.pos.x + deviation.valid_box.x
		|| std::abs(pos.z) > model.pos.z + deviation.valid_box.z;
}
