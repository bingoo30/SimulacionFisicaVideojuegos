#pragma once
#include <PxPhysicsAPI.h>
#include <vector>
#include <string>
#include <memory>
#include "Entity.h"
#include "ParticleSystem.h"
#include "StructForEntities.h"
#include "ForceRegistry.h"
#include "GravityForceGenerator.h"
class Scene{
public:
	Scene();
	virtual ~Scene();
	virtual void init() =0;
	virtual void update(double t);
	virtual void enter();
	virtual void exit();
	virtual void handle_input(unsigned char key)=0;

	void create_particle(const Particle_Data& pd);
	void create_projectile(const Projectile_Data& pd, Camera* c);
	const std::string& getDisplayText()const { return display; };
	static void add_particle_to_registry(Particle* p, ForceGenerator* f=nullptr);
protected:
	std::vector<std::unique_ptr<Entity>> gObjs;
	std::vector<std::unique_ptr<ParticleSystem>> gPartSys;
	std::string display;
	GravityForceGenerator* g; //gravedad: meto a todas las particulas
	ForceRegistry* gForceReg;
	static Scene* currentScene; //escena actual para el metodo estatico
};

