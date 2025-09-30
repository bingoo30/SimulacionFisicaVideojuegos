#pragma once
#include <PxPhysicsAPI.h>
#include "../RenderUtils.hpp"
class Particle {
public:
	Particle(const physx::PxVec3& pos, const physx::PxVec3& vel);
	~Particle() {};

	void integrate(double t);
private:
	physx::PxVec3 pos;
	physx::PxVec3 vel;

	RenderItem* renderItem;
};