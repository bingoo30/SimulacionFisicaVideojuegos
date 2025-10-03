#include "Scene.h"
#include "RenderUtils.hpp"
#include "Entity.h"
using namespace physx;
Scene::Scene(): gObjs()
{
}

Scene::~Scene()
{
	gObjs.clear();
}

void Scene::enter()
{
	for (auto o : gObjs) RegisterRenderItem(o->getRenderItem());
}

void Scene::exit()
{
	for (auto o : gObjs) DeregisterRenderItem(o->getRenderItem());
}
