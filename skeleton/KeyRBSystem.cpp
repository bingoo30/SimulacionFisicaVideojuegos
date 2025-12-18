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

void KeyRBSystem::reset()
{
    if (particles_list.empty()) return;

    auto* playerRB = static_cast<DynamicRigidBody*>(particles_list.front().get());
    PxRigidDynamic* actor = static_cast<PxRigidDynamic*>(playerRB->getActor());

    if (!actor) return;

    // Resetear a posición original
    PxTransform originalTransform(initPos);
    actor->setGlobalPose(originalTransform);

    // Resetear velocidad
    actor->setLinearVelocity(PxVec3(0, 0, 0));
    actor->setAngularVelocity(PxVec3(0, 0, 0));
}

void KeyRBSystem::setInitPos(const physx::PxVec3& p)
{
    initPos = p;
}
bool KeyRBSystem::check_out_of_limit(Particle* p) const
{
	return false;
}
