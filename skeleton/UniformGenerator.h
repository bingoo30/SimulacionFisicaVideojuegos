#pragma once
#include "Generator.h"
class UniformGenerator:public Generator {
public:
#pragma region constructoras y destructora
	UniformGenerator() = default;
	virtual ~UniformGenerator() = default;
#pragma endregion 
#pragma region metodos
	Particle_List generate_particles(const Particle_Data& model, const Particle_Deviation_Data& deviation, int n, physx::PxGeometryType::Enum geo) override;
#pragma endregion
};