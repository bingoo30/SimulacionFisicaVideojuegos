#pragma once
#include "Generator.h"
class UniformGenerator:public Generator {
public:
#pragma region constructoras y destructora
	UniformGenerator() :Generator() {};
	UniformGenerator(const physx::PxVec3& o, const physx::PxVec3& v, double d, int n) : Generator(o, v, d, n) {};
	virtual ~UniformGenerator() {};
#pragma endregion 
#pragma region metodos
	std::list<Particle*> generate_particles() const override;
#pragma endregion
};