#pragma once
#include "core.hpp"

class Particle;
class ForceGenerator
{
public:
	ForceGenerator()=default;
	virtual ~ForceGenerator();
	virtual void calculate_force(Particle* p) = 0;
	virtual void update_force(Particle* particle) = 0;
private:
	Vector3 _force;
};

