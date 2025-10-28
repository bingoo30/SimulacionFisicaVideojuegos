#pragma once
#include <PxPhysicsAPI.h>
#include <vector>
#include <string>
#include "Entity.h"
#include "ParticleSystem.h"
#include "StructForEntities.h"
class Scene{
public:
	Scene();
	virtual ~Scene();
	virtual void init() =0;
	virtual void update(double t);
	virtual void enter();
	virtual void exit();

	void create_particle(const Particle_Data& pd);
	void create_projectile(const Projectile_Data& pd, Camera* c);
	const std::string& getDisplayText()const { return display; };
protected:
	std::vector<Entity*> gObjs;
	std::vector<ParticleSystem*> gPartSys;
	std::string display;
};

