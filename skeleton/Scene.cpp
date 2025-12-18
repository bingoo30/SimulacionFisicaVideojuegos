#include "Scene.h"
#include "RenderUtils.hpp"
#include "Entity.h"
#include "Particle.h"
#include "Projectile.h"
#include "SceneManager.h"
#include "RubberBandForceGenerator.h"
#include <cmath>
#include <algorithm>
using namespace physx;
Scene::Scene(): gObjs(), gPartSys(), display("escena"), fRegistry(ForceRegistry()), gr(new GravityForceGenerator({ 0.0, -9.8, 0.0 })), gRBSys()
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
	for (auto rb : gRBSys) {
		rb->register_particles();
		rb->pause_all_rb(false);
	}
	for (auto f : gForcesWithRender) {
		f->register_aux_renders();
	}
}

void Scene::exit()
{
	for (auto e : gObjs) {
		if (e) {
			e->derregister_renderItem();
		}
	}
	for (auto ps : gPartSys) {
		if (ps)ps->derregister();
	}
	for (auto rb : gRBSys) {
		if (rb) {
			rb->derregister();
			rb->pause_all_rb(true);
		}

	}
	for (auto f : gForcesWithRender) {
		f->derregister_aux_renders();
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
	gObjs.push_back(part);
	return part;
}
Particle* Scene::create_capsule_particle(const Particle_Data& pd)
{
	Particle* part = new Particle(
		pd.pos,
		pd.color,
		pd.mass,
		CreateShape(physx::PxCapsuleGeometry(pd.vol*0.5, pd.vol*0.25)),
		pd.vol,
		pd.lifetime,
		pd.vel,
		pd.mode,
		pd.density);
	part->create_renderItem();
	gObjs.push_back(part);
	return part;
}
Particle* Scene::create_Platform(const Particle_Data& pd)
{
	// Crear la forma del cubo
	physx::PxShape* shape = CreateShape(physx::PxBoxGeometry(
		pd.scale.x,
		pd.scale.y,
		pd.scale.z));

	// Crear la partícula con esa forma
	Particle* part = new Particle(
		pd.pos,
		pd.color,
		pd.mass,
		shape,
		pd.vol,
		pd.lifetime,
		pd.vel,
		pd.mode,
		pd.density
	);

	//sin gravedad, es estatico
	gObjs.push_back(part);
	part->create_renderItem();
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
	gObjs.push_back(proj);
	return proj;
}

void Scene::create_slinky(Particle_Data& pd, const Spring_Data& sd, int N)
{
	Particle* prev = create_particle(pd);

	for (int i = 1; i < N; ++i)
	{
		double newC = std::max<double>(0.05 * i, 0.0);
		pd.color.x -= newC;
		pd.color.y -= newC;
		pd.color.z -= newC;
		pd.pos.y -= sd.resisting_length*1.1f;
		pd.pos.x += 1;

		Particle* p = create_particle(pd);

		// El RubberBand une p (actual) hacia prev (arriba)
		RubberBandForceGenerator* rb =
			new RubberBandForceGenerator(sd.k, sd.resisting_length, prev);

		// Registramos solo en p (la que debe sentir la fuerza)
		fRegistry.add_registry(p, rb);

		prev = p;
	}
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
void Scene::add_RB_system(RigidBodySystem* rbs)
{
	if (rbs != nullptr) {
		gRBSys.push_back(rbs);
	}
}
//
//void Scene::add_gravity_force_to(Particle* p)
//{
//	if (p != nullptr) {
//		fRegistry.add_registry(p, gr);
//	}
//}
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

	for (auto rb : gRBSys) {
		delete rb;
		rb = nullptr;
	}

	delete gr;
	gr = nullptr;
}

void Scene::update(double t) {
	fRegistry.update_forces(t);

	for (auto& e : gObjs) {
		if (e && e->is_alive()) e->update(t);
	}

	for (auto ps : gPartSys) {
		if (ps)ps->update(t); // cada sistema aplica sus propias fuerzas
	}

	for (auto ps : gRBSys) {
		if (ps)ps->update(t); // cada sistema aplica sus propias fuerzas
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