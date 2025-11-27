#include "Scene6.h"
#include "SpringForceGenerator.h"
#include "AnchoredSpringForceGenerator.h"
#include "ForceRegistry.h"
using namespace physx;
void Scene6::init()
{
	display = "escena 6: muelles";
	SpringData sd;

	sd.k = 1;
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
	fRegistry.add_registry(p2, muelleSinParticula);
}

void Scene6::handle_special_input(int key)
{
	muelleSinParticula->handle_special_input(key);
}

void Scene6::handle_input(unsigned char key)
{
	muelleSinParticula->handle_input(key);
}
