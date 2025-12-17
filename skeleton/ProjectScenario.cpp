#include "ProjectScenario.h"
#include "SceneManager.h"
#include "Render/Render.h"
#include "FireParticleSystem.h"
#include "RainParticleSystem.h"
#include "WhirlwindForceGenerator.h"
#include "WindForceGenerator.h"
#include <algorithm>

void ProjectScenario::init()
{
	explication_tramps = "Fuegos, gases toxicas, vientos, ...";
	instructions = "ENTER para continuar, 'Q' para retroceder...";
	display = "Cómo va a ser el escenario...";
	Platform_Data pd;
	pd.voly = 20.0;
	pd.volx = 50.0;
	pd.volz = 20.0;

	Fire_Particle_Data fpd;
	fpd.vol = 1.0;
	fpd.mass = 0.05;
	Fire_Deviation_Data fdd;
	fdd.ori = physx::PxVec3(3.5f, 0.0f, 3.5f);
	create_demo_platforms(pd, fpd, fdd, 2.5, 4);

	Whirlwind_Data wd;
	wd.center.x = -150.0;
	wd.area = 100.0;
	auto whirlwind = new WhirlwindForceGenerator(wd.center, wd.area, wd.k1, wd.dragCoef, wd.K, false);

	Wind_Data wind_d;
	wind_d.center = physx::PxVec3(400,80,0);
	wind_d.area = 50.0;
	auto wind = new WindForceGenerator(wind_d.center, wind_d.vel, wind_d.area, wind_d.k1, wind_d.dragCoef, false);

	Rain_Particle_Data rpd;
	rpd.mass = 0.1;
	rpd.vol = 0.3;
	rpd.pos = physx::PxVec3(0, 80, 0);
	Rain_Deviation_Data rdd;
	rdd.ori = physx::PxVec3(fpd.pos.x/2+80, 0, 40);

	RainParticleSystem* rps1 = new RainParticleSystem(rpd, rdd, 20);
	rps1->init();
	rps1->add_force_generator(whirlwind);
	gPartSys.push_back(rps1);

	rpd.pos.x = fpd.pos.x / 2 +150;
	rdd.ori.x = 80;
	rpd.color = physx::PxVec4(1.0, 1.0, 1.0, 1.0);
	RainParticleSystem* rps2 = new RainParticleSystem(rpd, rdd, 10);
	rps2->init();
	rps2->add_force_generator(wind);
	gPartSys.push_back(rps2);
}

void ProjectScenario::render_interface()
{
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	drawText(explication_tramps, width*0.05, height*0.8, 12, false, true);
	glColor4f(0.0f, 0.1f, 0.7f, 1.0f);
	drawText(instructions, width * 0.05, height * 0.9, 16, false, true);
}

void ProjectScenario::handle_input(unsigned char key)
{
	switch (toupper(key))
	{
	case ' ':
		break;
	case '\r': //tecla intro
		//SceneManager::instance().change_scene(States::DEMO_CHARACTER);
		break;
	case 'Q': //tecla Q
		SceneManager::instance().change_scene(States::INTRO);
		break;
	}
}

void ProjectScenario::create_demo_platforms(Platform_Data& pd, Fire_Particle_Data& fpd, const Fire_Deviation_Data& fdd, double offset, int count)
{
	int numGroups = 3;     // rojo, verde, morado
	int groupSize = 4;     // cada grupo tiene 4 tonos
	int baseIndex = fpd.color_offset;     // primer índice donde empiezan los colores personalizados (rojos)

	for (int i = 0; i < count; ++i) {
		create_Platform(pd);
		pd.pos.x += offset * pd.volx;

		fpd.pos.x = pd.pos.x - 65;

		if (i < count - 1) {
			auto f = new FireParticleSystem(fpd, fdd, 5+i);
			f->init();
			add_particle_system(f);
		}

		// Avanzar al siguiente grupo de color (rojo → verde → morado → rojo)
		int currentGroup = (fpd.color_offset - baseIndex) / groupSize;
		int nextGroup = (currentGroup + 1) % numGroups;
		fpd.color_offset = baseIndex + nextGroup * groupSize;
	}
}