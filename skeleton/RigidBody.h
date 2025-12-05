#pragma once
#include "Particle.h"
#include "StructForEntities.h"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"

//quiero meter materiales
class RigidBody : public Particle {
public:
	RigidBody(Particle_Data& pd, bool _static, physx::PxShape* shape, physx::PxMaterial* material = nullptr);
	void update(double dt) override;
	virtual void setPosition(const physx::PxVec3& p) = 0;
	virtual void setVelocity(const physx::PxVec3& v) = 0;
protected:
	bool is_static;
};