#pragma once
#include <PxPhysicsAPI.h>
#include <vector>
#include <string>
#include "Entity.h"
#include "ParticleSystem.h"
#include "ForceRegistry.h"
#include "GravityForceGenerator.h"
#include "StructForEntities.h"
class Scene{
public:
	Scene();
	virtual ~Scene();
	void clean();
	virtual void init() =0;
	virtual void update(double t);
	virtual void enter();
	virtual void exit();

	void create_particle(const Particle_Data& pd);
	void create_projectile(const Projectile_Data& pd, Camera* c);
	const std::string& getDisplayText()const { return display; };

	virtual void handle_input(unsigned char key) {};

	virtual void add_entity_with_renderItem(Entity* e);
	virtual void add_particle_system(ParticleSystem* ps);

	void add_gravity_force_to(Particle* p);
	GravityForceGenerator* getGravityGenerator() { return gr; };

protected:
	std::vector<Entity*> gObjs;
	std::vector<ParticleSystem*> gPartSys;

	std::string display;

	ForceRegistry* fRegistry;
	GravityForceGenerator* gr;
};

