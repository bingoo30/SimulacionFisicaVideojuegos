#include "FireRBSystem.h"
#include "NormalGeneratorRB.h"
using namespace physx;
FireRBSystem::FireRBSystem(const Fire_Particle_Data& pd, const Fire_Deviation_Data& pdd, int n, const physx::PxVec3& md, const physx::PxFilterData& dt) : RigidBodySystem(pd, pdd, n, PxGeometryType::eSPHERE, false, md, dt)
{
}
void FireRBSystem::init()
{
	spawn_acu = 0.05;
	auto n = new NormalGeneratorRB(false);
	n->setFilter(filter);
	//n->setTrigger(true);
	add_generator(n);
}

bool FireRBSystem::check_out_of_limit(Particle* p) const
{
	if (!p) return true;

	const physx::PxVec3& pos = p->getPosition();
	return pos.y <= 0.0 || pos.y > model.pos.y + deviation.valid_box.y
		|| std::abs(pos.x) > model.pos.x + deviation.valid_box.x
		|| std::abs(pos.z) > model.pos.z + deviation.valid_box.z;
}
