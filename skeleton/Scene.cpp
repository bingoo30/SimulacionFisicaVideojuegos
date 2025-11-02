#include "Scene.h"
#include "RenderUtils.hpp"
#include "Entity.h"
#include "Particle.h"
#include "Projectile.h"
#include "StructForEntities.h"
#include <cmath>
using namespace physx;
Scene::Scene() : gObjs(), gPartSys(), display("escena"), gForceReg(new ForceRegistry()), g(new GravityForceGenerator(CONST_GRAVITY))
{
}

Scene::~Scene()
{
	delete gForceReg;
}
void Scene::enter() {
	// Registrar todos los sistemas de partículas en el render
	for (auto& s : gPartSys) s->register_particles(); // método que crea los render items
	for (auto& o : gObjs) RegisterRenderItem(o->getRenderItem());
}

void Scene::exit()
{
	for (auto& s : gPartSys) s->derregister();
	for (auto& o : gObjs) DeregisterRenderItem(o->getRenderItem());
}
void Scene::create_particle(const Particle_Data& pd)
{
	Particle* part = new Particle(
		pd.pos,
		pd.color,
		pd.vel,
		pd.type,
		pd.mass,
		pd.life,
		CreateShape(physx::PxSphereGeometry(pd.vol)),
		pd.vol);
	gForceReg->add_registry(part, g);
	gObjs.push_back(std::make_unique<Particle>(part));

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
	float masa_sim = pd.masa * pow(pd.vel_real / pd.vel_sim, 2);

	// Aceleraci�n simulada ajustando la gravedad
	physx::PxVec3 acc_sim = pd.acc;
	acc_sim.y = pd.acc.y * pow(pd.vel_real / pd.vel_sim, 2);

	// Crear el proyectil
	Projectile* proj = new Projectile(
		startPos + forward * pd.offset,
		pd.color,
		vel_sim,
		acc_sim, 
		masa_sim,
		pd.vida,
		CreateShape(physx::PxSphereGeometry(pd.volumen))
	);
	gForceReg->add_registry(proj, g);
	gObjs.push_back(std::make_unique<Particle>(proj));
}
void Scene::update(double t) {
	for (auto& o : gObjs) o->update(t);
	for (auto& p : gPartSys) p->update(t);
}