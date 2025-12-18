#include "Victory.h"
#include "SceneManager.h"
#include "Render/Render.h"
#include "StructForEntities.h"
#include "BouyancyForceGenerator.h"
using namespace physx;
void Victory::init()
{
	title = "CONGRATULATIONS!";
	subtitle = "Pulsa ENTER para volver al menu de seleccion...";
	display = "Escena victoria";

	Liquid_Data ld; 
	BouyancyForceGenerator* bg = new BouyancyForceGenerator(ld.height, ld.volume, ld.density); 
	gForcesWithRender.push_back(bg); 

	Ground_Data pfd;
	pfd.pos = PxVec3(0, 15, 0);
	pfd.lifetime = -1;
	pfd.mass = 900.0;
	pfd.scale = { 1.0, 1.0, 1.0 };
	pfd.mode = SEMI_IMPLICIT_EULER;
	Particle* p3 = create_Platform(pfd);
	p3->setDamping(0.3);

	fRegistry.add_registry(p3, gr);
	fRegistry.add_registry(p3, bg);

}

void Victory::render_interface()
{
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	drawText(title, width * 0.5, height * 0.8, 64, true, true);
	glColor4f(1.0f, 0.2f, 0.2f, 1.0f);
	drawText(subtitle, width * 0.5, height * 0.5, 24);
}

void Victory::handle_input(unsigned char key)
{
	switch (toupper(key))
	{
	case ' ':
		break;
	case '\r': //tecla intro
		SceneManager::instance().change_scene(States::SELECTION);
		break;
	}
}
