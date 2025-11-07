#include "ProjectCharacterDemo.h"
#include "SceneManager.h"
#include "Render/Render.h"
#include <algorithm>
using namespace physx;
void ProjectCharacterDemo::init()
{
	explication_forces = "Mantener SPACE para acumular fuerza y cuando sueltas se aplica dicha fuerza";
	explication_inputs = "ENTER para volver a la pagina principal, 'Q' para retroceder...";
	display = "Cómo funciona el juego...";
	create_character();
}

void ProjectCharacterDemo::render_interface()
{
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	drawText(explication_forces, width*0.05, height*0.8, 12, false, true);
	glColor4f(0.0f, 0.1f, 0.7f, 1.0f);
	drawText(explication_inputs, width * 0.05, height * 0.9, 16, false, true);
}

void ProjectCharacterDemo::handle_input(unsigned char key)
{
	switch (toupper(key))
	{
	case ' ':
		if (character)
			character->setChargingForce(true);
		break;
	case '\r': //tecla intro
		SceneManager::instance().change_scene(States::INTRO);
		break;
	case 'Q': //tecla Q
		SceneManager::instance().change_scene(States::DEMO_SCENARIO);
		break;
	}
}

void ProjectCharacterDemo::handle_special_input(int key)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		character->move_left();
		break;
	case GLUT_KEY_RIGHT:
		character->move_right();
		break;
	}
}

void ProjectCharacterDemo::handle_key_up(unsigned char key)
{
	if (key == ' ' && character)
	{
		// Salto
		character->jump();
	}
}

void ProjectCharacterDemo::create_character()
{
	Particle_Data pd;
	pd.pos = physx::PxVec3(-20, 30, 0);
	pd.vel = physx::PxVec3(0, -20, 0);
	pd.mass = 30.0;
	pd.color = colors[19];
	pd.mode = VERLET;
	pd.lifetime = -1.0;
	pd.vol = 2.0;
	auto shape = CreateShape(physx::PxBoxGeometry(pd.vol, pd.vol, pd.vol));
	//shape->setLocalPose(PxTransform(PxQuat(PxHalfPi, PxVec3(0, 0, 1)))); 
	character = new Character(pd.pos, pd.color, pd.mass, shape, pd.vol, pd.lifetime, pd.vel, pd.mode);
	add_gravity_force_to(character);
	gObjs.push_back(character);
}
