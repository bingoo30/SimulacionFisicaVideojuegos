#include "Scene.h"
#include "RenderUtils.hpp"
#include "Entity.h"
#include "Particle.h"
#include "Projectile.h"
using namespace physx;
Scene::Scene(): gObjs()
{
}

Scene::~Scene()
{
	for (auto o : gObjs) delete o;
	gObjs.clear();
}
void Scene::create_particle(const Particle_Data& pd)
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
void Scene::create_projectile(const Projectile_Data& pd, Camera* c)
{
	// posición inicial = posición de la cámara
	physx::PxVec3 startPos = c->getTransform().p;

	// dirección = vector hacia donde apunta la cámara
	physx::PxVec3 forward = c->getDir();

	// el proyectil se crea un poco por delante de la cámara
	Projectile* proj = new Projectile(
		startPos + forward * pd.offset,  // posición inicial desplazada
		pd.color,
		forward * pd.vel,    // velocidad en la dirección de la cámara
		pd.acc,
		pd.damping,
		pd.tipo,
		pd.masa,
		pd.vida
	);

	gObjs.push_back(proj);
}
}
void Scene::enter()
{
	for (auto o : gObjs) RegisterRenderItem(o->getRenderItem());
}

void Scene::exit()
{
	for (auto o : gObjs) DeregisterRenderItem(o->getRenderItem());
}
void Scene::update(double t) {
	// integrar y comprobar tiempo de vida
	for (auto it = gObjs.begin(); it != gObjs.end();) {
		Entity* e = *it;
		if (e != nullptr) {
			e->update(t);

			if (e->isDead()) {
				DeregisterRenderItem(e->getRenderItem());
				delete e;
				it = gObjs.erase(it);
			}
			else {
				++it;
			}
		}
	}
}