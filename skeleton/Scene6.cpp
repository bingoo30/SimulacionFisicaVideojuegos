#include "Scene6.h"
#include "SpringForceGenerator.h"
#include "AnchoredSpringForceGenerator.h"
#include "RubberBandForceGenerator.h"
#include "BouyancyForceGenerator.h"
#include "ForceRegistry.h"
using namespace physx;
void Scene6::init()
{
	display = "escena 6: muelles";
	Spring_Data sd;

	sd.k = 2;
	sd.resisting_length = 5.0;
	//muelleSinParticula = new AnchoredSpringForceGenerator(sd.k, sd.resisting_length, PxVec3(-30, 50, 0)); 
	
	Particle_Data pd;
	pd.pos = PxVec3(3, 30, 0);
	pd.lifetime = -1;
	pd.mode = SEMI_IMPLICIT_EULER;
	//Particle* p = create_particle(pd);

	/*Platform_Data pld;
	pld.volx = pld.voly = pld.volz = 2;
	pld.pos = PxVec3(0, 30, 0);
	Particle* p1 = create_Platform(pld);

	muelleConParticula = new SpringForceGenerator(sd.k,sd.resisting_length, p1);

	pd.pos.x = -15;
	Particle* p2 = create_particle(pd);
	fRegistry.add_registry(p, muelleConParticula);
	fRegistry.add_registry(p, gr);
	fRegistry.add_registry(p2, muelleSinParticula);
	fRegistry.add_registry(p2, gr);
	Liquid_Data ld;
	BouyancyForceGenerator* bg = new BouyancyForceGenerator(ld.height, ld.volume, ld.density);
	gForcesWithRender.push_back(bg);

	Platform_Data pfd;
	pfd.pos = PxVec3(0, 15, 0);
	pfd.lifetime = -1;
	pfd.mass = 900.0;
	pfd.volx = pfd.voly = pfd.volz = 1.0;
	pfd.mode = SEMI_IMPLICIT_EULER;
	Particle* p3 = create_Platform(pfd);
	p3->setDamping(0.3);

	fRegistry.add_registry(p3, gr);
	fRegistry.add_registry(p3, bg);*/

	create_slinky(pd, sd, 6);
}

void Scene6::handle_special_input(int key)
{
	//muelleSinParticula->handle_special_input(key);
}

void Scene6::handle_input(unsigned char key)
{
	//muelleSinParticula->handle_input(key);
	Particle_Data pd;
	pd.vel = { 10, 10, 0 };
	muelleConParticula->handle_input(key);
	switch (toupper(key))
	{
	case ' ':
		break;
	case 'K':
		pd.color = { 0,1,0,1 };
		pd.mode = SEMI_IMPLICIT_EULER;
		Particle* p1 = create_particle(pd);
		fRegistry.add_registry(p1, gr);
		pd.pos.x = -10;
		pd.color.x = 1;
		Spring_Data sd;

		RubberBandForceGenerator* rb = new RubberBandForceGenerator(sd.k, sd.resisting_length, p1);
		Particle* p2 = create_particle(pd);
		fRegistry.add_registry(p2, rb);
		fRegistry.add_registry(p2, gr);
		break;
	}
}
