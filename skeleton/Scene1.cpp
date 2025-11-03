#include "Scene1.h"
#include "Entity.h"
#include "FireParticleSystem.h"


void Scene1::init()
{
	Fire_Particle_Data fpd;
	fpd.pos = Vector3(0, 0, 0);
	Fire_Deviation_Data fdd;

	FireParticleSystem* fps = new FireParticleSystem(fpd, fdd, 1);
	gPartSys.push_back(std::unique_ptr<FireParticleSystem>(fps));
}

void Scene1::handle_input(unsigned char key)
{
}
