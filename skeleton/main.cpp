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
#include "Scene3.h"
#include "Scene4.h"
#include "SceneManager.h"

#include "ProjectIntro.h"
#include "ProjectScenario.h"
#include "ProjectCharacterDemo.h"
std::string display_text = "";


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




void updateDisplay() {
	display_text = SceneManager::instance().getCurrScene()->getDisplayText();
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


	//practica
	////myScenes.push_back(new Scene1());
	//Scene* s0 = new Scene0();
	//s0->init();
	//SceneManager::instance().add(s0);
	//Scene* s1 = new Scene1();
	//s1->init();
	//SceneManager::instance().add(s1);
	//Scene* s2 = new Scene2();
	//s2->init();
	//SceneManager::instance().add(s2);
	//Scene* s3 = new Scene3();
	//s3->init();
	//SceneManager::instance().add(s3);
	//Scene* s4 = new Scene4();
	//s4->init();
	//SceneManager::instance().add(s4);

	//SceneManager::instance().set_initial_scene(0);


	//proyecto
	Scene* intro = new ProjectIntro();
	intro->init();
	SceneManager::instance().add(intro);

	Scene* scenario = new ProjectScenario();
	scenario->init();
	SceneManager::instance().add(scenario);

	Scene* demo = new ProjectCharacterDemo();
	demo->init();
	SceneManager::instance().add(demo);

	SceneManager::instance().set_initial_scene(States::INTRO);
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

	SceneManager::instance().getCurrScene()->update(t);
	updateDisplay();
	std::this_thread::sleep_for(std::chrono::microseconds(10));
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	//unregister all render items
	DeregisterAllRenderItems();
	SceneManager::instance().clean();

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
// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);
	SceneManager::instance().getCurrScene()->handle_input(key);
	switch(toupper(key))
	{
	case ' ':
		break;
	default:
		//if (key >= '0' && key <= '9') {
		//	int newScene = key - '0';  // conversión de char a int ('0'→0, '1'→1, ...)
		//	SceneManager::instance().change_scene(newScene);
		//	updateDisplay();
		//}
		break;
	}
}
// Function called when a key is up
void keyUp(unsigned char key)
{
	SceneManager::instance().getCurrScene()->handle_key_up(key);
}
// Function called when a special key is pressed
void specialKeyPress(int key)
{
	SceneManager::instance().getCurrScene()->handle_special_input(key);
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