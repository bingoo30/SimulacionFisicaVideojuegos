#include "Scene0.h"

void Scene0::init()
{
	display = "escena 0: invocar particulas y proyectiles";
	explication = "PARTICULAS: J euler, K semi implicit, L verlet; Proyectiles: C canon, P bala";
}

void Scene0::handle_input(unsigned char key)
{
	Particle_Data pd;
	pd.vel = { 10, 10, 0 };
	Pistol_Bullet_Data pbd;
	Cannon_Data cd;
	switch (toupper(key))
	{
	case ' ':
	case 'J':
		pd.color = { 1,0,0,1 };
		pd.mode = EULER;
		fRegistry.add_registry(create_particle(pd), gr);
		break;
	case 'K':
		pd.color = { 0,1,0,1 };
		pd.mode = SEMI_IMPLICIT_EULER;
		fRegistry.add_registry(create_particle(pd), gr);
		break;
	case 'L':
		pd.color = { 0,0,1,1 };
		pd.mode = VERLET;
		fRegistry.add_registry(create_particle(pd), gr);
		break;
	case 'P':
		fRegistry.add_registry(create_projectile(pbd, GetCamera()), gr);
		break;
	case 'C':
		fRegistry.add_registry(create_projectile(cd, GetCamera()), gr);
		break;
	}
}
void Scene0::render_interface()
{
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	drawText(explication, width * 0.1, height * 0.8, 12, false, false);
}
