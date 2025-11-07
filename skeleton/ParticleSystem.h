#pragma once
#include "Particle.h"
#include "Generator.h"
#include "ForceGenerator.h"
#include <list>
#include <memory>
#include "StructForEntities.h"
#include"ForceRegistry.h"
class ParticleSystem
{
public:
#pragma region constructoras y destructora
	ParticleSystem(const Particle_Data& pd, const Particle_Deviation_Data& pdd, int n, physx::PxGeometryType::Enum geo);
	virtual ~ParticleSystem();
#pragma endregion

#pragma region metodos publicos
	void spawn();
	void update(double dt);
	void derregister();
	void register_particles(); 
	void add_generator(Generator* gen);
	void add_force_generator(ForceGenerator* gen);
#pragma endregion

protected:
#pragma region atributos
	std::list<std::unique_ptr<Particle>> particles_list;
	std::list<Generator*> generators;
	std::list<std::unique_ptr<ForceGenerator>> force_generators;
	ForceRegistry local_registry;

	//particula modelo
	Particle_Data model;
	Particle_Deviation_Data deviation;
	int num;

	double spawn_acu =0.0;
	double spawn_period=0.0;

	physx::PxGeometryType::Enum geometry;
#pragma endregion
	virtual bool check_out_of_limit(Particle* p) const = 0;
	virtual void kill_dead_particles();
};

