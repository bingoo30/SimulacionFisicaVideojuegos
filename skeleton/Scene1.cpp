#include "Scene1.h"
#include "Entity.h"
#include "FireParticleSystem.h"
Scene1::Scene1(): Scene()
{
}

void Scene1::init()
{
	Fire_Particle_Data fpd;
	fpd.pos = Vector3(0, 0, 0);
	Fire_Deviation_Data fdd;

	FireParticleSystem* fps = new FireParticleSystem(fpd, fdd, 5);
	gPartSys.push_back(fps);
}
