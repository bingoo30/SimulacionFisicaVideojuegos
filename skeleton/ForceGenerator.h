#pragma once
#include "core.hpp"
class Particle;
class ForceGenerator
{
public:
	ForceGenerator()=default;
	virtual ~ForceGenerator() = default;
	virtual void update_force(Particle* particle) = 0;
protected:
	virtual physx::PxVec3 calculate_force(Particle* p) = 0;
};

