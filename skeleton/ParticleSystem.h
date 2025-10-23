#pragma once
#include "Particle.h"
#include "Generator.h"
#include <list>
#include "StructForEntities.h"
class ParticleSystem
{
public:
#pragma region constructoras y destructora
	ParticleSystem(Generator* g, const Particle_Data& pd, const Particle_Deviation_Data& pdd, int n, physx::PxGeometryType::Enum geo);
	virtual ~ParticleSystem();
#pragma endregion

#pragma region getters y setters
	Generator* getGenerator() { return gen; };
	void setGenerator(Generator* g) { gen = g;};
#pragma endregion

#pragma region metodos publicos
	void spawn();
	void update(double dt);
	void derregister();
	void register_particles(); 
#pragma endregion

protected:
#pragma region atributos
	Generator* gen;
	std::list<Particle*> particles_list;

	Particle_Data model;
	Particle_Deviation_Data deviation;
	int num;

	double spawnAccumulator;
	double spawnInterval;

	physx::PxGeometryType::Enum geometry;
#pragma endregion
	virtual bool check_out_of_limit(Particle* p) const = 0;
};

