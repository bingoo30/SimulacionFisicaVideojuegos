#include "DynamicRigidBody.h"
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

void DynamicRigidBody::update(double dt)
{
    //Actualizar tiempo de vida
    update_lifetime(dt);

    //Si está muerto, no hacer nada
    if (!check_death()) return;

    //Aplicar fuerzas acumuladas

	//physx siempre usa el integrador semi-implicito
    if (force != physx::PxVec3(0.0f)) {
		body->wakeUp(); // asegurar que no esté dormido
		body->addForce(force, PxForceMode::eFORCE, true);
        force = physx::PxVec3(0.0f); // limpiar fuerza
    }

	// Sincronizar posición y rotación para render
	PxTransform t = body->getGlobalPose();
	transform.p = PxVec3(t.p.x, t.p.y, t.p.z);
	transform.q = PxQuat(t.q.x, t.q.y, t.q.z, t.q.w);
}
