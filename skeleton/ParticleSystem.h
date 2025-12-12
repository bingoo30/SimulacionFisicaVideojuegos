#pragma once
#include "Particle.h"
#include "Generator.h"
#include "ForceGenerator.h"
#include <list>
#include <vector>
#include <memory>
#include "StructForEntities.h"
#include"ForceRegistry.h"
class ParticleSystem
{
public:
#pragma region constructoras y destructora
	ParticleSystem(const Particle_Data& pd, const Particle_Deviation_Data& pdd, int n, physx::PxGeometryType::Enum geo, const physx::PxVec3& md = physx::PxVec3(0));
	virtual ~ParticleSystem();
	virtual void init() = 0;
#pragma endregion

#pragma region metodos publicos
	virtual void spawn(bool withRender = true);
	virtual void update(double dt);
	void derregister();
	void register_particles(); 
	void add_force_generator(ForceGenerator* gen);
	bool is_active() const;
	void set_Active(bool a);
#pragma endregion

protected:
#pragma region atributos
	std::list<std::unique_ptr<Particle>> particles_list;
	std::vector<Generator*> generators;
	std::list<std::unique_ptr<ForceGenerator>> force_generators;
	ForceRegistry local_registry;

	//particula modelo
	Particle_Data model;
	Particle_Deviation_Data deviation;
	int num;

	double spawn_acu =0.0;
	double spawn_period=0.0;

	physx::PxGeometryType::Enum geometry;
	bool active;

	physx::PxMaterial* mat;
#pragma endregion
	void add_generator(Generator* gen);
	virtual bool check_out_of_limit(Particle* p) const = 0;
	virtual void kill_dead_particles();
};

