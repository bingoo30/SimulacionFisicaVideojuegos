#include "Scene.h"
#include "RenderUtils.hpp"
#include "Entity.h"
#include "Particle.h"
#include "Projectile.h"
#include <cmath>
using namespace physx;
Scene::Scene(): /*gObjs()*/ gPartSys()
{
}

Scene::~Scene()
{
	for (auto s : gPartSys) delete s;
	gPartSys.clear();
}
void Scene::enter() {
	// Registrar todos los sistemas de partículas en el render
	for (auto s : gPartSys) {
		s->register_particles(); // método que crea los render items
	}
}
void Scene::exit()
{
	for (auto s : gPartSys) s->derregister();
}
void Scene::update(double t) {
	for (auto p : gPartSys) p->update(t);
}