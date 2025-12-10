#pragma once
#include "RigidBody.h"
#include "StructForEntities.h"

class DynamicRigidBody : public RigidBody {
public:
	DynamicRigidBody(const Particle_Data& prop,
		physx::PxFilterData& _filter,
		physx::PxShape* shape,
		physx::PxMaterial* _material = nullptr);
	virtual ~DynamicRigidBody();
	#pragma region getters
		inline const physx::PxVec3& getPosition() const override { return body->getGlobalPose().p; };
		inline const physx::PxVec3& getVelocity() const override { return body->getLinearVelocity(); };
		inline physx::PxActor* getActor() override { return body; }
	#pragma endregion
	#pragma region setters
		inline void setPosition(const physx::PxVec3& p) override { body->setGlobalPose(PxTransform(p)); };
		inline void setVelocity(const physx::PxVec3& v) override { body->setLinearVelocity(v); };
		inline void setTensor(const physx::PxVec3& t) { body->setMassSpaceInertiaTensor(t); };
	#pragma endregion
		void add_force(const physx::PxVec3& f) override;
protected:
	physx::PxRigidDynamic* body;
	physx::PxMaterial* material;
	physx::PxVec3 tensor;
	physx::PxFilterData filter;
};