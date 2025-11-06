#include "Scene.h"
#include "RenderUtils.hpp"
#include "Entity.h"
#include "Particle.h"
#include "Projectile.h"
#include "SceneManager.h"
#include <cmath>
using namespace physx;
Scene::Scene(): gObjs(), gPartSys(), display("escena"), fRegistry(new ForceRegistry()), gr(new GravityForceGenerator({ 0.0, -9.8, 0.0 }))
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
Particle* Scene::create_particle(const Particle_Data& pd)
{
	Particle* part = new Particle(
		pd.pos,
		pd.color,
		pd.mass,
		CreateShape(physx::PxSphereGeometry(pd.vol)),
		pd.vol,
		pd.lifetime,
		pd.vel,
		pd.mode,
		pd.density);
	part->create_renderItem();
	SceneManager::instance().getCurrScene()->add_gravity_force_to(part);
	gObjs.push_back(part);
	return part;
}
Projectile* Scene::create_projectile(const Projectile_Data& pd, Camera* c)
{
	//posicion de la camara como posicion inicial
	PxVec3 startPos = c->getTransform().p;

	//direccion;
	PxVec3 forward = c->getDir().getNormalized();

	//velocidad real
	PxVec3 vr = forward * pd.vel_real;

	//velocidad simulada para que sea visible en pantalla
	PxVec3 vel_sim = forward * pd.vel_sim;

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
		pd.mode,
		pd.density
	);
	proj->create_renderItem();
	SceneManager::instance().getCurrScene()->add_gravity_force_to(proj);
	gObjs.push_back(proj);
	return proj;
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

void Scene::add_gravity_force_to(Particle* p)
{
	if (p != nullptr) {
		fRegistry->add_registry(p, gr);
	}
}
void Scene::add_force_to(Particle* p, ForceGenerator* f)
{
	if (p != nullptr) {
		fRegistry->add_registry(p, f);
	}
}
void Scene::clean()
{
	for (auto e : gObjs) {
		delete e;
		e = nullptr;
	}
	gObjs.clear();

	for (auto ps : gPartSys) {
		delete ps;
		ps = nullptr;
	}
	gPartSys.clear();

	delete fRegistry;
	fRegistry = nullptr;

	delete gr;
	gr = nullptr;
}

void Scene::update(double t) {
	fRegistry->update_forces(t);

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