#include "Scene.h"
#include "RenderUtils.hpp"
#include "Entity.h"
#include "Particle.h"
#include "Projectile.h"
#include "StructForEntities.h"
#include <cmath>
#include <iostream>
using namespace physx;
using namespace std;
Scene* Scene::currentScene = nullptr;
Scene::Scene() : gObjs(), gPartSys(), display("escena"), gForceReg(new ForceRegistry()), g(new GravityForceGenerator(CONST_GRAVITY))
{
	currentScene = this;
}

Scene::~Scene()
{
	delete gForceReg;
	if (currentScene == this)
		currentScene = nullptr;
}
void Scene::enter() {
	// Registrar todos los sistemas de partículas en el render
	for (auto& s : gPartSys) s->register_particles(); // método que crea los render items
	for (auto& o : gObjs) o->create_renderItem();
}

void Scene::exit()
{
	for (auto& s : gPartSys) s->derregister();
	for (auto& o : gObjs) o->derregister_renderItem();
}
void Scene::create_particle(const Particle_Data& pd)
{
	Particle* part = new Particle(
		pd.pos,
		pd.color,
		pd.mass,
		CreateShape(PxSphereGeometry(pd.vol)),
		pd.vol,
		pd.life,
		pd.vel,
		pd.type);
	part->create_renderItem();
	gForceReg->add_registry(part, g);
	gObjs.push_back(unique_ptr<Particle>(part));

}
void Scene::create_projectile(const Projectile_Data& pd, Camera* c)
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
	physx::PxVec3 acc_sim = pd.acc;
	acc_sim.y = pd.acc.y * pow(pd.vel_real / pd.vel_sim, 2);

	// Crear el proyectil
	Projectile* proj = new Projectile(
		startPos + forward * pd.offset,
		pd.color,
		masa_sim,
		CreateShape(PxSphereGeometry(pd.vol)),
		pd.vol,
		pd.life,
		vel_sim
		//acc_sim, 
	);
	proj->create_renderItem();
	gForceReg->add_registry(proj, g);
	gObjs.push_back(unique_ptr<Particle>(proj));
}
void Scene::add_particle_to_registry(Particle* p, ForceGenerator* f)
{
	if (currentScene == nullptr) {
		std::cerr << "No hay una escena activa\n";
		return;
	}

	if (currentScene->gForceReg) {
		if (f != nullptr) currentScene->gForceReg->add_registry(p, currentScene->g);
		else if (currentScene->g) currentScene->gForceReg->add_registry(p, currentScene->g);
	}
	else {
		std::cerr << "gForceReg o g o f no están inicializados.\n";
	}
}
void Scene::update(double t) {
	gForceReg->update_forces();

	for (auto& o : gObjs) o->update(t);
	// eliminar muertos manualmente
	gObjs.erase(
		std::remove_if(gObjs.begin(), gObjs.end(),
			[](const std::unique_ptr<Entity>& o) {
				if (o->is_dead()) {
					return true;
				}
				return false;
			}),
		gObjs.end()
	);

	for (auto& p : gPartSys) p->update(t);
}