#pragma once
#include <PxPhysicsAPI.h>
#include <vector>
#include <string>
#include "Entity.h"
#include "ParticleSystem.h"
#include "RigidBodySystem.h"
#include "ForceRegistry.h"
#include "GravityForceGenerator.h"
#include "StructForEntities.h"
#include "Projectile.h"
class Scene{
public:
	Scene();
	virtual ~Scene();
	void clean();
	virtual void init() =0;
	virtual void update(double t);
	virtual void render_interface(){};
	virtual void enter();
	virtual void exit();

	virtual Particle* create_particle(const Particle_Data& pd);
	virtual Particle* create_capsule_particle(const Particle_Data& pd);
	virtual Particle* create_Platform(const const Particle_Data& pd);
	virtual Projectile* create_projectile(const Projectile_Data& pd, Camera* c);
	void create_slinky(Particle_Data& pd, const Spring_Data& sd, int N);
	const std::string& getDisplayText()const { return display; };

	virtual void handle_input(unsigned char key) {};
	virtual void handle_special_input(int key) {};
	virtual void handle_key_up(unsigned char key) {};
	virtual void handle_special_key_up(int key) {};

	virtual void add_entity_with_renderItem(Entity* e);
	virtual void add_particle_system(ParticleSystem* ps);
	virtual void add_RB_system(RigidBodySystem* rbs);
	//void add_gravity_force_to(Particle* p);
	GravityForceGenerator* getGravityGenerator() { return gr; };

protected:
	std::vector<Entity*> gObjs;
	std::vector<ParticleSystem*> gPartSys;

	std::string display;

	ForceRegistry fRegistry;
	//vector auxiliar para pintar las fuerzas
	std::vector<ForceGenerator*> gForcesWithRender;
	GravityForceGenerator* gr;

	std::vector<RigidBodySystem*> gRBSys;
};

