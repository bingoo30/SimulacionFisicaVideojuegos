#pragma once
#include <PxPhysicsAPI.h>
#include <vector>
#include "Entity.h"
#include "StructForEntities.h"
class Scene{
public:
	Scene();
	virtual ~Scene();
	virtual void init() = 0;
	virtual void enter();
	virtual void update(double t) = 0;
	virtual void exit();

	void create_particle(Particle_Data& pd);

protected:
	std::vector<Entity*> gObjs;
};

