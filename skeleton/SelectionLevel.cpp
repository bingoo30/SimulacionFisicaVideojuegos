#include "SelectionLevel.h"
#include "SceneManager.h"
#include "ButtonSystem.h"
void SelectionLevel::init()
{
	title = "Pulsa INTRO para seleccionar nivel";
	display = "Seleccion de niveles";
	selectedLevel = States::LEVEL1;
	Button_Data bt;
	bt.lifetime = -1;
	ParticleSystem* but = new ButtonSystem(bt, "txt/selectionButtons.txt");
	but->init();
	add_particle_system(but);
}

void SelectionLevel::render_interface()
{
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	drawText(title, width * 0.5, height * 0.9, 50, true, true);
}

void SelectionLevel::handle_special_input(int key)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		if (selectedLevel != States::LEVEL1) selectedLevel--;
		break;
	case GLUT_KEY_RIGHT:
		if (selectedLevel != States::LEVEL2) selectedLevel++;
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
