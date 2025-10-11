#pragma once
#include "Generator.h"
class NormalGenerator:public Generator {
public:
#pragma region constructoras y destructora
	NormalGenerator() :Generator() {};
	NormalGenerator(const physx::PxVec3& o, const physx::PxVec3& v, double d, int n) : Generator(o, v, d, n) {};
	virtual ~NormalGenerator() {};
#pragma endregion 
#pragma region metodos
	std::list<Particle*> generate_particles(const physx::PxVec3& pos_dev,
		const physx::PxVec3& vel_dev,
		double mas_dev,
		double dur_dev,
		bool rand_color, 
		bool rand_cant) override;
#pragma endregion
};