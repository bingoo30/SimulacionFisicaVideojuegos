#pragma once
#include "RigidBody.h"
#include "StructForEntities.h"

class StaticRigidBody : public RigidBody {
public:
	StaticRigidBody(Particle_Data& pd, physx::PxFilterData filter, physx::PxShape* shape, physx::PxMaterial* material = nullptr);
	virtual ~StaticRigidBody();
	const physx::PxVec3& getPosition() const override;
	void setPosition(const physx::PxVec3& p) override;
protected:
	physx::PxRigidStatic* body;
};