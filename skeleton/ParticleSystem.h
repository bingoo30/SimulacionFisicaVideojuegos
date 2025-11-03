#pragma once
#include "Particle.h"
#include "Generator.h"
#include <list>
#include <memory>
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
	virtual void update(double dt);
	void derregister();
	void register_particles(); 
	void add_particle(Particle* p);
#pragma endregion
protected:
#pragma region atributos
	//generador
	Generator* gen;
	//lista de particulas
	mutable Particle_List particles_list;

	//particula modelo
	Particle_Data model;
	//desviacion modelo
	Particle_Deviation_Data deviation;
	//numero de particulas que va a generar
	int num;

	//tiempo acumulado desde la ultima vez que spawneo
	double spawn_acu;
	//cada cuantos segundos spawneo
	double spawn_period;

	//tipo de geometria
	physx::PxGeometryType::Enum geometry;
#pragma endregion
#pragma region metodos auxiliares
	virtual bool check_out_of_limit(Particle* p) const = 0;
	virtual void update_particles(double dt);
	virtual void check_spawn(double dt);
#pragma endregion
};

