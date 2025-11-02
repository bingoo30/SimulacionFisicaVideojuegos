#include "Scene0.h"

void Scene0::init()
{
	display = "escena 0: invocar particulas y proyectiles";

}

void Scene0::handle_input(unsigned char key)
{
	Particle_Data pd;
	pd.vel = { 10, 10, 0 };
	switch (toupper(key))
	{
	case ' ':
		break;
	case 'J':
		pd.color = { 1,0,0,1 };
		pd.type = Entity::EULER;
		create_particle(pd);
		break;
	case 'K':
		pd.color = { 0,1,0,1 };
		pd.type = Entity::EULER_SEMIIMPLICIT;
		create_particle(pd);
		break;
	case 'L':
		pd.color = { 0,0,1,1 };
		pd.type = Entity::VERLET;
		create_particle(pd);
		break;
	case 'P':
		Pistol_Bullet_Data pbd;
		create_projectile(pbd, GetCamera());
		break;
	case 'C':
		Cannon_Data cd;
		create_projectile(cd, GetCamera());
		break;
	}
}
