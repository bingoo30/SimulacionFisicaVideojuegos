#include "Scene4.h"
#include "Entity.h"
#include "FireParticleSystem.h"
#include "RainParticleSystem.h"
#include "StructForEntities.h"
#include "FootBallSystem.h"
using namespace physx;
void Scene4::init()
{
	display = "escena 4: particulas con torbellino";
	Whirlwind_Data wd; Explosion_Data ed; Wind_Data windD;
	
	wd.area = 50.0;
	windD.area = 50.0f;
	ed.center = PxVec3(10,0,0);
	windD.center = PxVec3(-10, 0, 0);

	whirlwind = new WhirlwindForceGenerator(wd.center, wd.area, wd.k1, wd.dragCoef, wd.K, false);
	explosion = new ExplosionForceGenerator(ed.center, ed.radius, ed.K, ed.tau);
	wind = new WindForceGenerator(windD.center, windD.vel, windD.area, windD.k1, windD.dragCoef, false);

	//Rain_Particle_Data rpd;
	//rpd.mass = 0.1;
	//rpd.vol = 0.3;
	//rpd.pos = Vector3(0, 80, 0);
	//Rain_Deviation_Data rdd;

	//RainParticleSystem* rps = new RainParticleSystem(rpd, rdd, 20);
	//rps->add_force_generator(gr);
	//rps->add_force_generator(whirlwind);
	//gPartSys.push_back(rps);

	explication = "E: explosion, R:Torbellino, T: viento, Y: gravedad";
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
		break;
	case 'Y':
	{
		Particle* p1 = create_particle(pd);
		fRegistry.add_registry(p1, gr);
		break;
	}
	case 'T':
	{
		Particle* p2 = create_projectile(pbd, GetCamera());
		fRegistry.add_registry(p2, wind);
		break;
	}
	case 'R':
	{
		Particle* p3 = create_projectile(cd, GetCamera());
		fRegistry.add_registry(p3, whirlwind);
		break;
	}
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
			pd.mass = 0.2f;

			auto p = Scene::create_particle(pd);
			fRegistry.add_registry(p, explosion);
		}
		explosion->activate(true);
		break;
	}
}

void Scene4::render_interface()
{
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	drawText(explication, width * 0.7, height * 0.8, 12, false, false);
}
