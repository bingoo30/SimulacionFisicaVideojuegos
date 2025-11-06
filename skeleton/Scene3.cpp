#include "Scene3.h"
#include "Entity.h"
#include "FireParticleSystem.h"
#include "RainParticleSystem.h"
#include "StructForEntities.h"
void Scene3::init()
{
	display = "escena 3: particulas con viento";
	Wind_Data wd;
	wind = new WindForceGenerator(physx::PxVec3(0.0), wd.vel, wd.area, wd.k1, wd.dragCoef, false);

	Fire_Particle_Data fpd;
	fpd.pos = physx::PxVec3(0, 0, 0);
	fpd.mass = 0.05;
	Fire_Deviation_Data fdd;
	fdd.vel = physx::PxVec3(0.0, 1.0, 0.0);

	FireParticleSystem* fps = new FireParticleSystem(fpd, fdd, 5);
	fps->add_force_generator(wind);
	gPartSys.push_back(fps);

	//Rain_Particle_Data rpd;
	//rpd.pos = Vector3(0, 80, 0);
	//Rain_Deviation_Data rdd;

	//RainParticleSystem* rps = new RainParticleSystem(rpd, rdd, 20);
	//rps->add_force_generator(wind);
	//gPartSys.push_back(rps);
}

Particle* Scene3::create_particle(const Particle_Data& pd)
{
	Particle* p = Scene::create_particle(pd);
	fRegistry->add_registry(p, wind);
	return p;
}

Projectile* Scene3::create_projectile(const Projectile_Data& pd, Camera* c)
{
	Projectile* pr = Scene::create_projectile(pd, c);
	Particle* p = static_cast<Particle*>(pr);
	fRegistry->add_registry(p, wind);
	return pr;
}

void Scene3::handle_input(unsigned char key)
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
	}
}