#include "Scene7.h"
#include "FootBallSystem.h"
#include <iostream>
using namespace physx;

void Scene7::init()
{
	//modelo de la pelota que voy a generar
	Football_Data fd;
	fd.pos.y = 40;
	Football_Deviation_Data fdd;
	fdd.ori.x = fdd.ori.z = 3;
	//crear el material de futbol
	FootBallSystem* system = new FootBallSystem(fd, fdd, 1, footballMaterial);
	system->init();
	add_RB_system(system);

	//probar
	auto g = std::make_unique<physx::PxSphereGeometry>(3);
	PxShape* sh = CreateShape(*g);

	PxFilterData f(0, 0, 0, 0);
	DynamicRigidBody* prueba = new DynamicRigidBody(fd, f, sh);
	fRegistry.add_registry(prueba, gr);
	gObjs.push_back(prueba);
}

void Scene7::handle_input(unsigned char key)
{
	switch (toupper(key))
	{
	case ' ':
		break;
	case 'P':
		gRBSys[0]->spawn();
		std::cout << "creados!\n";
		break;
	}
}
