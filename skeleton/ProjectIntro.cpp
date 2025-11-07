#include "ProjectIntro.h"
#include "SceneManager.h"
#include "Render/Render.h"
void ProjectIntro::init()
{
	title = "PROYECTO INTERMEDIO";
	subtitle = "Pulsa ENTER para empezar con el proyecto...";
	detail1 = "Pulsa 3 para probar disparo de particulas y proyectiles";
	detail2 = "Pulsa 4 para probar generadores de fuerzas";
	display = "Bingcheng Wang | Simulación física de videojuegos";
}

void ProjectIntro::render_interface()
{
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	drawText(title, width*0.5, height*0.8, 64, true, true);
	glColor4f(1.0f, 0.2f, 0.2f, 1.0f);
	drawText(subtitle, width * 0.5, height * 0.5, 24);
	glColor4f(1.0f, 0.2f, 0.2f, 1.0f);
	drawText(detail1, width * 0.5, height * 0.4, 24);
	glColor4f(1.0f, 0.2f, 0.2f, 1.0f);
	drawText(detail2, width * 0.5, height * 0.3, 24);

}

void ProjectIntro::handle_input(unsigned char key)
{
	switch (toupper(key))
	{
	case ' ':
		break;
	case '\r': //tecla intro
		SceneManager::instance().change_scene(States::DEMO_SCENARIO);
		break;
	}
}
