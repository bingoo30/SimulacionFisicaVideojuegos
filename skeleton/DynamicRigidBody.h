#pragma once
#include "RigidBody.h"
#include "StructForEntities.h"

class DynamicRigidBody : public RigidBody {
public:
	DynamicRigidBody(Particle_Data& pd, const physx::PxVec3& t= physx::PxVec3(-1), physx::PxFilterData filter = filterDataOther);PxMaterial* mat = nullptr);
	virtual ~DynamicRigidBody();

#pragma region getters
	const physx::PxVec3& getPosition() const override;
	const physx::PxVec3& getVelocity() const override;
	physx::PxActor* getActor() { return body; }
#pragma endregion

	void setPosition(const physx::PxVec3& p) override;
	void setVelocity(const physx::PxVec3& p) override;
protected:
	physx::PxRigidDynamic* body;
	physx::PxMaterial* material;
	physx::PxVec3 tensor;
	physx::PxFilterData filter;
};