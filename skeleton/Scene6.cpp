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
	/*SpringData sd;

	sd.k = 2;
	sd.resisting_length = 15.0;
	muelleSinParticula = new AnchoredSpringForceGenerator(sd.k, sd.resisting_length, PxVec3(-10, 0, 0)); 
	
	Particle_Data pd;
	pd.pos = PxVec3(3, 0, 0);
	pd.lifetime = -1;
	pd.mode = SEMI_IMPLICIT_EULER;
	Particle* p = create_particle(pd);

	Platform_Data pld;
	pld.volx = pld.voly = pld.volz = 2;
	pld.pos = PxVec3(0.0);
	Particle* p1 = create_Platform(pld);

	muelleConParticula = new SpringForceGenerator(sd.k,sd.resisting_length, p1);

	pd.pos.x = -15;
	Particle* p2 = create_particle(pd);
	fRegistry.add_registry(p, muelleConParticula);
	fRegistry.add_registry(p2, muelleSinParticula);*/
	LiquidData ld;
	BouyancyForceGenerator* bg = new BouyancyForceGenerator(ld.height, ld.volume, ld.density);
	gForcesWithRender.push_back(bg);

	Particle_Data pd;
	pd.pos = PxVec3(0, 2, 0);
	pd.lifetime = -1;
	pd.mass = 1000.0;
	pd.mode = SEMI_IMPLICIT_EULER;
	Particle* p3 = create_particle(pd);

	fRegistry.add_registry(p3, gr);
	fRegistry.add_registry(p3, bg);
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
		SpringData sd;
		sd.k = 1; sd.resisting_length = 2.0;

		RubberBandForceGenerator* rb = new RubberBandForceGenerator(sd.k, sd.resisting_length, p1);
		Particle* p2 = create_particle(pd);
		fRegistry.add_registry(p2, rb);

		break;
	}
}
