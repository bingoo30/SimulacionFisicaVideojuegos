#include "Scene2.h"
#include "Entity.h"
#include "RainParticleSystem.h"
//#include "FireworkParticleSystem.h"

void Scene2::init()
{
	Rain_Particle_Data rpd;
	rpd.pos = Vector3(0, 80, 0);
	Rain_Deviation_Data rdd;

	RainParticleSystem* rps = new RainParticleSystem(rpd, rdd, 20);
	gPartSys.push_back(std::unique_ptr<RainParticleSystem>(rps));

	Firework_Rocket_Data frd;
	Firework_Explosion_Deviation fed;
	rpd.pos = Vector3(0, 0, 0);

	/*FireworkParticleSystem* fwps = new FireworkParticleSystem(frd, fed, 5);
	gPartSys.push_back(std::unique_ptr<FireworkParticleSystem>(fwps));*/
}

void Scene2::handle_input(unsigned char key)
{
	switch (toupper(key))
	{
	case 'F':
		/*static_cast<FireworkParticleSystem*>(gPartSys[1].get())->launch_firework();*/
		break;
	}
}
