#pragma once
#include <PxPhysicsAPI.h>
#include <vector>

class Entity;
class Scene{
public:
	Scene();
	virtual ~Scene();
	virtual void init() = 0;
	virtual void enter();
	virtual void update() = 0;
	virtual void exit();

protected:
	std::vector<Entity*> gObjs;
};

