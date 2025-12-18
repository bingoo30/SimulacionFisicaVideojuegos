#include "Scene7.h"
#include "FootBallSystem.h"
#include "GroundSystem.h"
#include "WindForceGenerator.h"
#include "CharacterRBSystem.h"
#include <iostream>
using namespace physx;

void Scene7::init()
{
	display = "escena 7: solidos rigidos";
	//modelo de la pelota que voy a generar
	Football_Data fd;
	fd.pos.y = 40;
	Football_Deviation_Data fdd;
	fdd.ori.x = fdd.ori.z = 3;
	//crear el material de futbol
	FootBallSystem* system = new FootBallSystem(fd, fdd, 1, footballMaterial);
	system->init();
	add_RB_system(system);

	Player_Data player_model;
	player_model.pos.y = 50;
	player_model.scale *= 5;
	//CharacterRBSystem* ch = new CharacterRBSystem(player_model, characterMaterial, PxFilterData(0,0,0,0));
	//ch->init();
	//add_RB_system(ch);

	Ground_Data gd;
	GroundSystem* g = new GroundSystem(gd, 1);
	g->init();
	g->spawn(false);
	add_RB_system(g);

	//probar si le afecta la fuerza
	Wind_Data wd;
	wd.vel.x = 50;
	auto wind = new WindForceGenerator(physx::PxVec3(0, 30, 0), wd.vel, wd.area, wd.k1, wd.dragCoef, false);
	system->add_force_generator(wind);
	//probar
	/*auto g = std::make_unique<physx::PxSphereGeometry>(3);
	PxShape* sh = CreateShape(*g);

	PxFilterData f(0, 0, 0, 0);
	DynamicRigidBody* prueba = new DynamicRigidBody(fd, f, sh);
	fRegistry.add_registry(prueba, gr);
	gObjs.push_back(prueba);*/
}

void Scene7::handle_input(unsigned char key)
{
	switch (toupper(key))
	{
	case ' ':
		break;
	case 'P':
		gRBSys[0]->spawn();
		//std::cout << "creados!\n";
		break;
	case 'E':
		gRBSys[1]->spawn();
		//std::cout << "creados!\n";
		break;
	}
}
