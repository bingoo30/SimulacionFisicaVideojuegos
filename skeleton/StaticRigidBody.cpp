#include "StaticRigidBody.h"
using namespace physx;

extern PxPhysics* gPhysics;
extern PxScene* gScene;
StaticRigidBody::StaticRigidBody(const Particle_Data& pd, physx::PxFilterData filter, physx::PxShape* shape, physx::PxMaterial* material):
	RigidBody(pd, true, shape, material)
{
	shape->setSimulationFilterData(filter);
	body = gPhysics->createRigidStatic(PxTransform(transform.p));
	body->attachShape(*shape);
	gScene->addActor(*body);
}
StaticRigidBody::~StaticRigidBody()
{
	if (body != nullptr) 
		body->release();
}

physx::PxActor* StaticRigidBody::getActor()
{
	return body;
}
;
