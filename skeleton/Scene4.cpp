#include "Scene4.h"
#include "Entity.h"
#include "FireParticleSystem.h"
#include "RainParticleSystem.h"
#include "StructForEntities.h"
using namespace physx;
void Scene4::init()
{
	display = "escena 4: particulas con torbellino";
	Whirlwind_Data wd; Explosion_Data ed;
	
	whirlwind = new WhirlwindForceGenerator(wd.center, wd.area, wd.k1, wd.dragCoef, wd.K, false);
	explosion = new ExplosionForceGenerator(ed.center, ed.radius, ed.K, ed.tau);

	Rain_Particle_Data rpd;
	rpd.pos = Vector3(0, 80, 0);
	Rain_Deviation_Data rdd;

	RainParticleSystem* rps = new RainParticleSystem(rpd, rdd, 20);
	rps->add_force_generator(whirlwind);
	gPartSys.push_back(rps);
}

Particle* Scene4::create_particle(const Particle_Data& pd)
{
	Particle* p = Scene::create_particle(pd);
	fRegistry->add_registry(p, whirlwind);
	return p;
}

Projectile* Scene4::create_projectile(const Projectile_Data& pd, Camera* c)
{
	Projectile* pr = Scene::create_projectile(pd, c);
	Particle* p = static_cast<Particle*>(pr);
	fRegistry->add_registry(p, whirlwind);
	return pr;
}

void Scene4::handle_input(unsigned char key)
{
	Particle_Data pd;
	pd.vel = { 10, 10, 0 };
	Pistol_Bullet_Data pbd;
	Cannon_Data cd;
	switch (toupper(key))
	{
	case ' ':
	case 'J':
		pd.color = { 1,0,0,1 };
		pd.mode = EULER;
		create_particle(pd);
		break;
	case 'K':
		pd.color = { 0,1,0,1 };
		pd.mode = SEMI_IMPLICIT_EULER;
		create_particle(pd);
		break;
	case 'L':
		pd.color = { 0,0,1,1 };
		pd.mode = VERLET;
		create_particle(pd);
		break;
	case 'P':
		create_projectile(pbd, GetCamera());
		break;
	case 'C':
		create_projectile(cd, GetCamera());
		break;
	case 'E':
		//Particle(
		//	const physx::PxVec3 & p, // posición
		//	const physx::PxVec4 & c, // color
		//	float m,     // masa
		//	physx::PxShape * sh, //tipo de geometria que uso,
		//	double vol,
		//	double lt, //tiempo de vida
		//	const physx::PxVec3 & v, // velocidad
		//	IntegrateMode md,     // tipo de integración,
		//	double de = 1.225, //densidad del material (este es el de aire)
		//	double d = 0.95 // damping
		//);
		Particle_Data pd;
		for (int i = 0; i < 10; ++i) {
			double angle = i * (360.0 / 10) * (PI / 180.0);
			double radius = 3.0;
			double x = cos(angle) * radius;
			double z = sin(angle) * radius;

			pd.pos = PxVec3(x, 10, z);
			pd.mass = 1.0f*i;

			auto p = Scene::create_particle(pd);
			fRegistry->add_registry(p, explosion);
		}
		explosion->activate(true);
		break;
	}
}