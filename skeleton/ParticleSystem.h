#pragma once
#include "Particle.h"
#include "Generator.h"
#include <list>
#include "StructForEntities.h"
class ParticleSystem
{
public:
#pragma region constructoras y destructora
	ParticleSystem(Generator* g, const Particle_Data& pd, const Particle_Deviation_Data& pdd, int n);
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
#pragma endregion

protected:
#pragma region atributos
	Generator* gen;
	std::list<Particle*> particles_list;

	Particle_Data model;
	Particle_Deviation_Data deviation;
	int num;
#pragma endregion

#pragma region metodos auxiliares
	void erase();
#pragma endregion
};

