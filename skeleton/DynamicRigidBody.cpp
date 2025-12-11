#include "DynamicRigidBody.h"
#include <iostream>
using namespace physx;

extern PxPhysics* gPhysics;
extern PxScene* gScene;

DynamicRigidBody::DynamicRigidBody(const Particle_Data& prop, physx::PxFilterData& _filter, physx::PxShape* shape, physx::PxMaterial* _material)
:RigidBody(prop, false, shape, _material),
body(nullptr), material(_material), filter(_filter), tensor(prop.tensor)
{
	shape->setSimulationFilterData(filter);
	body = gPhysics->createRigidDynamic(PxTransform(prop.pos));

	body->setLinearVelocity(prop.vel);
	body->setLinearDamping(prop.damping);
	body->setAngularVelocity(PxVec3(0));
	body->attachShape(*shape);
	gScene->addActor(*body);

	// Si le paso un tensor calculado manualmente, se lo asigno
	if (tensor != PxVec3(-1)) setTensor(tensor);
	// Si no, PhysX lo calcula automáticamente según la geometría y masa
	else PxRigidBodyExt::setMassAndUpdateInertia(*body, prop.mass);
}
DynamicRigidBody::~DynamicRigidBody()
{
	if (body != nullptr) {
		body->release();
		body = nullptr;
	}
}

void DynamicRigidBody::create_renderItem()
{
	derregister_renderItem();
	renderItem = std::make_unique<RenderItem>(shape, body, color);
	renderItemRegisted = true;
}

void DynamicRigidBody::add_force(const physx::PxVec3& f)
{
	body->addForce(f);
}

void DynamicRigidBody::add_torque(const physx::PxVec3& t)
{
	body->addTorque(t);
}

void DynamicRigidBody::update(double dt)
{
	RigidBody::update(dt);
	std::cout << "posicion: " << getPosition().x <<" " << getPosition().y << " " << getPosition().z << "\n";
}
