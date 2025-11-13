#include "Scene5.h"
#include "Entity.h"
#include "FireworkParticleSystem.h"
#include "StructForEntities.h"
using namespace physx;
void Scene5::init()
{
	display = "escena 5: fuegos artificiales";

	Firework_Particle_Data fwpd;
	Firework_Deviation_Data fwdd;

	FireworkParticleSystem* fwps = new FireworkParticleSystem(fwpd, fwdd);
	fwps->init();
	Explosion_Data ed;
	Explosion_De
}

void Scene5::handle_input(unsigned char key)
{
	Particle_Data pd;
	pd.vel = { 10, 10, 0 };
	Pistol_Bullet_Data pbd;
	Cannon_Data cd;
	switch (toupper(key))
	{
	case ' ':
		break;
	case 'F':

		break;
	}

}

