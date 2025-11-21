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
	sd.resisting_length = 10.0;
	AnchoredSpringForceGenerator* muelleSinParticula = new AnchoredSpringForceGenerator(sd.k, sd.resisting_length, PxVec3(0.0)); 
	
	Particle_Data pd;
	pd.pos = PxVec3(3, 0, 0);
	pd.lifetime = -1;
	pd.mode = SEMI_IMPLICIT_EULER;
	Particle* p = create_particle(pd);
	gObjs.push_back(p);

	Platform_Data pld;
	pld.volx = pld.voly = pld.volz = 2;
	pld.pos = PxVec3(0.0);
	Particle* p1 = create_Platform(pld);
	gObjs.push_back(p1);

	SpringForceGenerator* muelleConParticula = new SpringForceGenerator(sd.k,sd.resisting_length, p1);

	fRegistry.add_registry(p, muelleConParticula);
}

void Scene6::handle_special_input(int key)
{
}

void Scene6::handle_input(unsigned char key)
{
}
