#include "GameOver.h"
#include "SceneManager.h"
#include "Render/Render.h"
#include "StructForEntities.h"
#include "WhirlwindForceGenerator.h"
#include "FootBallSystem.h"

using namespace physx;
void GameOver::init()
{
	title = "YOU LOSE...";
	subtitle1 = "Pulsa ENTER para volver a intentarlo...";
	subtitle2 = "Pulsa ESC para salir";
	display = "Escena GameOver";

	Whirlwind_Data wd; 

	wd.area = 50.0;

	whirlwind = new WhirlwindForceGenerator(wd.center, wd.area, wd.k1, wd.dragCoef, wd.K, false);

	Football_Data fd; Football_Deviation_Data fdd;
	fd.lifetime = 0.5; fdd.valid_box = { 100, 100, 100 };
	FootBallSystem* foot = new FootBallSystem(fd, fdd, 3, footballMaterial);
	foot->add_force_generator(whirlwind);
}

void GameOver::render_interface()
{
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	drawText(title, width * 0.5, height * 0.8, 64, true, true);
	glColor4f(0.5f, 0.2f, 0.2f, 1.0f);
	drawText(subtitle1, width * 0.5, height * 0.5, 24);
	glColor4f(1.0f, 0.2f, 0.2f, 1.0f);
	drawText(subtitle2, width * 0.5, height * 0.3, 24);
	glColor4f(1.0f, 0.2f, 0.2f, 1.0f);
}

void GameOver::handle_input(unsigned char key)
{
	switch (toupper(key))
	{
	case ' ':
		break;
	case '\r': //tecla intro
		SceneManager::instance().change_scene(States::INTRO);
		break;
	}
}

