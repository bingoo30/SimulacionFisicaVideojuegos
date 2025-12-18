#include "Victory.h"
#include "SceneManager.h"
#include "Render/Render.h"
void Victory::init()
{
	title = "CONGRATULATIONS!";
	subtitle = "Pulsa ENTER para volver al menu de seleccion...";
	display = "Escena victoria";
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
