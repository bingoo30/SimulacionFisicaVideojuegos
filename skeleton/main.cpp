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
#include "Scene0.h"
#include "Scene1.h"
#include "Scene2.h"

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


void updateDisplay() {
	display_text = myScenes[sceneAct]->getDisplayText();
}
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
	sceneDesc.gravity = CONST_GRAVITY;
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	//parte 2 practica 0
	//RenderItem* centro = new RenderItem(CreateShape(PxSphereGeometry(2)), new PxTransform(0.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	//RegisterRenderItem(centro);

	//RenderItem* bolaX = new RenderItem(CreateShape(PxSphereGeometry(1)), new PxTransform(20.0f, 0.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	//RegisterRenderItem(bolaX);
	//RenderItem* bolaY = new RenderItem(CreateShape(PxSphereGeometry(1)), new PxTransform(0.0f, 20.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	//RegisterRenderItem(bolaY);
	//RenderItem* bolaZ = new RenderItem(CreateShape(PxSphereGeometry(1)), new PxTransform(0.0f, 0.0f, 20.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	//RegisterRenderItem(bolaZ);

	//myScenes.push_back(new Scene1());
	Scene* s0 = new Scene0();
	s0->init();
	myScenes.push_back(s0);
	Scene* s1 = new Scene1();
	s1->init();
	myScenes.push_back(s1);
	Scene* s2 = new Scene2();
	s2->init();
	myScenes.push_back(s2);
	sceneAct = 0;
	updateDisplay();
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
	updateDisplay();
}
// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);
	myScenes[sceneAct]->handle_input(key);
	switch(toupper(key))
	{
	case ' ':
		break;
	case '0':
	case '1':
	case '2':
		int newScene = key - '0'; // conversion de char a int
		changeScene(newScene);
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