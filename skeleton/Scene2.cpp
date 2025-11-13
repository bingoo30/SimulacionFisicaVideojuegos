#include "Scene2.h"
#include "Entity.h"
#include "RainParticleSystem.h"

void Scene2::init()
{
	display = "escena 2: sistema de particulas: lluvia";
	Rain_Particle_Data rpd;
	rpd.pos = Vector3(0, 80, 0);
	Rain_Deviation_Data rdd;

	RainParticleSystem* rps = new RainParticleSystem(rpd, rdd, 20);
	rps->init();
	gPartSys.push_back(rps);
}
