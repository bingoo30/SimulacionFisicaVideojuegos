#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>
#include <cstdlib>
#include "Particle.h"

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

std::vector<Particle*> particles;

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
	/*RenderItem* centro = new RenderItem(CreateShape(PxSphereGeometry(2)), new PxTransform(0.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	RegisterRenderItem(centro);

	RenderItem* bolaX = new RenderItem(CreateShape(PxSphereGeometry(1)), new PxTransform(20.0f, 0.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	RegisterRenderItem(bolaX);
	RenderItem* bolaY = new RenderItem(CreateShape(PxSphereGeometry(1)), new PxTransform(0.0f, 20.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	RegisterRenderItem(bolaY);
	RenderItem* bolaZ = new RenderItem(CreateShape(PxSphereGeometry(1)), new PxTransform(0.0f, 0.0f, 20.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	RegisterRenderItem(bolaZ);*/
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	for (auto p : particles) p->integrate(t);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	//unregister all render items
	DeregisterAllRenderItems();
	particles.clear();

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

	switch(toupper(key))
	{
	case '1': //euler
		particles.push_back(new Particle(
			PxVec3(0), 
			PxVec4(0,1,0,1), 
			PxVec3(10, 15, 0), 
			PxVec3(0, -9.8, 0), 
			0.999, Particle::EULER));
		break;
	case '2': //semi
		particles.push_back(new Particle(
			PxVec3(0), 
			PxVec4(1,0,0,1), 
			PxVec3(10, 15, 0), 
			PxVec3(0, -9.8, 0), 
			0.999, 
			Particle::EULER_SEMIIMPLICIT));
		break;
	case '3': //verlet
		particles.push_back(new Particle(
			PxVec3(0), 
			PxVec4(0,0,1,1), 
			PxVec3(5, 10, 0), 
			PxVec3(0, -9.8, 0), 
			0.999, 
			Particle::VERLET));
		break;
	case ' ':
	{
		break;
	}
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