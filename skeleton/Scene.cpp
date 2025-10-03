#include "Scene.h"
#include "RenderUtils.hpp"
#include "Entity.h"
#include "Particle.h"
using namespace physx;
Scene::Scene(): gObjs()
{
}

Scene::~Scene()
{
	for (auto o : gObjs) delete o;
	gObjs.clear();
}
void Scene::create_particle(Particle_Data& pd)
{
	Particle* part = new Particle(
		pd.pos,
		pd.color,
		pd.vel,
		pd.acc,
		pd.damping, 
		pd.tipo,
		pd.masa,
		pd.vida);
	gObjs.push_back(part);

}
void Scene::enter()
{
	for (auto o : gObjs) RegisterRenderItem(o->getRenderItem());
}

void Scene::exit()
{
	for (auto o : gObjs) DeregisterRenderItem(o->getRenderItem());
}
