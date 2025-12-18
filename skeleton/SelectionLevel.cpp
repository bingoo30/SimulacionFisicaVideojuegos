#include "SelectionLevel.h"
#include "SceneManager.h"
#include "ButtonSystem.h"
void SelectionLevel::init()
{
	title = "Pulsa INTRO para seleccionar nivel";
	subtitle = "Pulsa q para volver";
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
	level = "Nivel " + std::to_string(selectedLevel-States::LEVEL1 + 1);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	drawText(title, width * 0.5, height * 0.9, 50, true, true);
	if (selectedLevel == States::LEVEL1) {
		glColor4f(0.0f, 0.8f, 0.0f, 1.0f);
		drawText(level, width * 0.25, height * 0.55, 32, true, true);
	}
	else {
		glColor4f(0.0f, 0.0f, 0.8f, 1.0f);
		drawText(level, width * 0.75, height * 0.55, 32, true, true);
	}

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	drawText(subtitle, width * 0.5, height * 0.7, 50, true, true);
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
	case 'Q': //tecla q
		SceneManager::instance().change_scene(INTRO);
		break;
	}
}
