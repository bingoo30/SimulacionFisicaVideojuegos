#include "Scene1.h"
#include "Entity.h"
#include "FireParticleSystem.h"


void Scene1::init()
{
	display = "escena 1: sistema de particulas: fuego";
	Fire_Particle_Data fpd;
	fpd.pos = Vector3(0, 0, 0);
	Fire_Deviation_Data fdd;

	FireParticleSystem* fps = new FireParticleSystem(fpd, fdd, 1);
	gPartSys.push_back(fps);
}
