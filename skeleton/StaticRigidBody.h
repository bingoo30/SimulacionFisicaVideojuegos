#pragma once
#include "RigidBody.h"
#include "StructForEntities.h"

class StaticRigidBody : public RigidBody {
public:
	StaticRigidBody(const Particle_Data& pd, physx::PxFilterData filter, physx::PxShape* shape, physx::PxMaterial* material = nullptr);
	virtual ~StaticRigidBody();
    #pragma region getters
	inline const physx::PxVec3& getPosition() const override { return body->getGlobalPose().p; };
	inline physx::PxRigidActor* getActor() override { return body; };
#pragma endregion
    #pragma region setters
	inline void setPosition(const physx::PxVec3& p) override { body->setGlobalPose(physx::PxTransform(p)); };
#pragma endregion
protected:
	physx::PxRigidStatic* body;
};