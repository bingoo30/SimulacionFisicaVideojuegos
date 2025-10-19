#pragma once
#include "Generator.h"
class UniformGenerator:public Generator {
public:
#pragma region constructoras y destructora
	UniformGenerator() = default;
	virtual ~UniformGenerator() = default;
#pragma endregion 
#pragma region metodos
	std::list<Particle*> generate_particles(const Particle_Data& model, const Particle_Deviation_Data& deviation, int n) override;
#pragma endregion
};