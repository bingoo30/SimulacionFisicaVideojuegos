#pragma once
#include <random>
#include <list>
#include "Particle.h"
#include "StructForEntities.h"
class Generator {
public:
	Generator() {};
	virtual ~Generator() {};

	virtual std::list<Particle*>& generate_particles (const Particle_Data& model, const Particle_Deviation_Data& deviation, int n) = 0;
protected:
	std::mt19937 _mt; 
};
