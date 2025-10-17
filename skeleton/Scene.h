#pragma once
#include <PxPhysicsAPI.h>
#include <vector>
#include "Entity.h"
#include "ParticleSystem.h"
#include "StructForEntities.h"
class Scene{
public:
	Scene();
	virtual ~Scene();
	virtual void init() = 0;
	virtual void update(double t);
	virtual void exit();

	//void create_particle(const Particle_Data& pd);
	//void create_projectile(const Projectile_Data& pd, Camera* c);

protected:
	//std::vector<Entity*> gObjs;
	std::vector<ParticleSystem*> gPartSys;
};

