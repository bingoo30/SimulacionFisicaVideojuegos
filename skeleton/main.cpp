#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>
#include <random>
#include <chrono>
#include <thread>

#include "Particle.h"
#include "StructForEntities.h"
#include "Scene.h"
#include "Scene1.h"

std::string display_text = "Practica 1";


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

std::vector<Scene*> myScenes;
int sceneAct = 0;
// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	//parte 2 practica 0
	RenderItem* centro = new RenderItem(CreateShape(PxSphereGeometry(2)), new PxTransform(0.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	RegisterRenderItem(centro);

	RenderItem* bolaX = new RenderItem(CreateShape(PxSphereGeometry(1)), new PxTransform(20.0f, 0.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	RegisterRenderItem(bolaX);
	RenderItem* bolaY = new RenderItem(CreateShape(PxSphereGeometry(1)), new PxTransform(0.0f, 20.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	RegisterRenderItem(bolaY);
	RenderItem* bolaZ = new RenderItem(CreateShape(PxSphereGeometry(1)), new PxTransform(0.0f, 0.0f, 20.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	RegisterRenderItem(bolaZ);

	myScenes.push_back(new Scene1());
	myScenes.push_back(new Scene1());
	sceneAct = 0;
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	myScenes[sceneAct]->update(t);
	std::this_thread::sleep_for(std::chrono::microseconds(10));
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	//unregister all render items
	DeregisterAllRenderItems();
	myScenes.clear();

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------

	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();
}
void changeScene(int newS) {
	if (newS >= myScenes.size() || newS == sceneAct) return;
	
	myScenes[sceneAct]->exit();
	sceneAct = newS;
	myScenes[sceneAct]->enter();
}
// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);
	Particle_Data pd;
	pd.vel = {10, 10, 0};
	Pistol_Bullet_Data pbd;
	Cannon_Data cd;
	switch(toupper(key))
	{
	case ' ':
		break;
	case 'J':
		pd.color = { 1,0,0,1 }; 
		pd.tipo = Entity::EULER;
		myScenes[sceneAct]->create_particle(pd);
		break;
	case 'K':
		pd.color = { 0,1,0,1 };
		pd.tipo = Entity::EULER_SEMIIMPLICIT;
		myScenes[sceneAct]->create_particle(pd);
		break;
	case 'L':
		pd.color = { 0,0,1,1 };
		pd.tipo = Entity::VERLET;
		myScenes[sceneAct]->create_particle(pd);
		break;
	case 'P':
		myScenes[sceneAct]->create_projectile(pbd, GetCamera());
		break;
	case 'C':
		myScenes[sceneAct]->create_projectile(cd, GetCamera());
		break;
	case '0':
		changeScene(0);
		break;
	case '1':
		changeScene(1);
		break;
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}