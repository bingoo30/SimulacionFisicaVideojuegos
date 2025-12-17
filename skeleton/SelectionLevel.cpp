#include "SelectionLevel.h"
#include "SceneManager.h"
#include "ButtonSystem.h"
void SelectionLevel::init()
{
	title = "Pulsa INTRO para seleccionar nivel";
	display = "Seleccion de niveles";

	Button_Data bt;
	ParticleSystem* but = new ButtonSystem(bt, "txt/selectionButtons.txt");
	add_particle_system(but);
}

void SelectionLevel::render_interface()
{
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	drawText(title, width * 0.5, height * 0.2, 50, true, true);
}

void SelectionLevel::handle_special_input(int key)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		if (selectedLevel != firstLevel) selectedLevel--;
		break;
	case GLUT_KEY_RIGHT:
		if (selectedLevel != lastLevel) selectedLevel++;
		break;
	}
}
void SelectionLevel::handle_input(unsigned char key)
{
	switch (toupper(key))
	{
	case ' ':
		break;
	case '\r': //tecla intro
		SceneManager::instance().change_scene(selectedLevel);
		break;
	}
}
