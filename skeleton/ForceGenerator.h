#pragma once
#include "core.hpp"

class Particle;
class ForceGenerator
{
public:
	ForceGenerator() : _force() {};
	virtual ~ForceGenerator();
	virtual void calculate_force(Particle* p) = 0;

private:
	Vector3 _force;
};

