#include "GameOver.h"
#include "SceneManager.h"
#include "Render/Render.h"
#include "StructForEntities.h"
#include "WhirlwindForceGenerator.h"
#include "FireParticleSystem.h"
#include "RainParticleSystem.h"

using namespace physx;
void GameOver::init()
{
	title = "YOU LOSE...";
	subtitle1 = "Pulsa ENTER para volver a intentarlo...";
	subtitle2 = "Pulsa ESC para salir";
	display = "Escena GameOver";

	Whirlwind_Data wd; 

	wd.area = 150.0;
	wd.center.x = -35;
	wd.center.y = 20;
	whirlwind = new WhirlwindForceGenerator(wd.center, wd.area, wd.k1, wd.dragCoef, wd.K, false);

	Firework_Particle_Data fd; Firework_Deviation_Data fdd;
	fd.color_offset = 12;
	fd.color_tam = 4;
	fd.lifetime = 3.5;
	fdd.r_color = true;
	fd.mass = 12.5;
	fd.pos.x = -60.0;
	fdd.valid_box = { 1000, 1000, 1000 };
	FireParticleSystem* fire = new FireParticleSystem(fd, fdd, 2);
	fire->init();
	add_particle_system(fire);
	
	fd.pos.x = 55;
	fd.color_offset = 16;
	FireParticleSystem* fire2 = new FireParticleSystem(fd, fdd, 1);
	fire2->init();
	add_particle_system(fire2);

	Rain_Particle_Data rpd; Rain_Deviation_Data rdd;
	rpd.pos.y = 30;
	rpd.mass = 1;
	rpd.color_offset = 4;
	rpd.color_tam = 16;
	rdd.r_color = true;
	RainParticleSystem* rain = new RainParticleSystem(rpd, rdd, 5);
	rain->init();
	gPartSys.push_back(rain);
	rain->add_force_generator(whirlwind);
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

