#include "Scene.h"
#include "RenderUtils.hpp"
#include "Entity.h"
#include "Particle.h"
#include "Projectile.h"
#include <cmath>
using namespace physx;
Scene::Scene(): gObjs(), gPartSys(), display("escena"), fRegistry(new ForceRegistry())
{
}

Scene::~Scene()
{
	clean();
}
void Scene::enter() {
	// Registrar todos los sistemas de partículas en el render
	for (auto e : gObjs) {
		if (e && !e->is_valid_renderItem()) {
			e->create_renderItem();
		}
	}
	for (auto ps : gPartSys) {
		ps->register_particles();
	}
}

void Scene::exit()
{
	for (auto e : gObjs) {
		if (e) e->derregister_renderItem();
	}
	for (auto ps : gPartSys) {
		if (ps)ps->derregister();
	}
}
void Scene::create_particle(const Particle_Data& pd)
{
	Particle* part = new Particle(
		pd.pos,
		pd.color,
		pd.mass,
		CreateShape(physx::PxSphereGeometry(pd.vol)),
		pd.vol,
		pd.lifetime,
		pd.vel,
		pd.mode);
	part->create_renderItem();
	gObjs.push_back(part);

}
void Scene::create_projectile(const Projectile_Data& pd, Camera* c)
{
	//posicion de la camara como posicion inicial
	physx::PxVec3 startPos = c->getTransform().p;

	//direccion;
	physx::PxVec3 forward = c->getDir().getNormalized();

	//velocidad real
	physx::PxVec3 vr = forward * pd.vel_real;

	//velocidad simulada para que sea visible en pantalla
	physx::PxVec3 vel_sim = forward * pd.vel_sim;

	//masa simulada para conservar la energia cinetica
	//NOTA: al tener el vector forward normalizado, su modulo es 1, 
	//por lo que el modulo de la velocidad real es exactamente la vel_real que hemos introducido
	float masa_sim = pd.mass * pow(pd.vel_real / pd.vel_sim, 2);

	// Aceleraci�n simulada ajustando la gravedad
	/*physx::PxVec3 acc_sim = pd.acc;
	acc_sim.y = pd.acc.y * pow(pd.vel_real / pd.vel_sim, 2);*/

	// Crear el proyectil
	Projectile* proj = new Projectile(
		startPos + forward * pd.offset,
		pd.color,
		masa_sim,
		CreateShape(physx::PxSphereGeometry(pd.vol)),
		pd.vol,
		pd.lifetime,
		vel_sim,
		pd.mode
	);
	proj->create_renderItem();
	gObjs.push_back(proj);
}

void Scene::add_entity_with_renderItem(Entity* e)
{
	if (e->getRenderItem() == nullptr) {
		e->create_renderItem();
	}
	gObjs.push_back(e);
}

void Scene::add_particle_system(ParticleSystem* ps)
{
	if (ps != nullptr) {
		gPartSys.push_back(ps);
	}
}

void Scene::clean()
{
	for (auto e : gObjs) delete e;
	gObjs.clear();

	for (auto ps : gPartSys) delete ps;
	gPartSys.clear();

	delete fRegistry;
}

void Scene::update(double t) {
	fRegistry->update_forces();

	for (auto& e : gObjs) {
		if (e)e->update(t);
	}

	for (auto ps : gPartSys) {
		if (ps)ps->update(t);
	}

	for (auto it = gObjs.begin(); it != gObjs.end(); ) {
		if (!(*it)->is_alive()) {
			delete* it;
			it = gObjs.erase(it);
		}
		else {
			++it;
		}
	}
}