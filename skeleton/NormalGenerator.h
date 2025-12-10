#pragma once
#include "Generator.h"

class NormalGenerator:public Generator {
public:
#pragma region constructoras y destructora
	NormalGenerator() = default;
	virtual ~NormalGenerator() = default;
#pragma endregion 
#pragma region metodos
	Particle_List generate_particles(const Particle_Data& model, const Particle_Deviation_Data& deviation, int n, physx::PxGeometryType::Enum geo, physx::PxMaterial* _mat = nullptr) override;
#pragma endregion
};