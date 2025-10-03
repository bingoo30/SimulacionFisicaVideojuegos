#pragma once
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include "Entity.h"
//fichero auxiliar que guardan los structs(datos) necesarios necesarios para inicializar un objeto de la escena
struct Particle_Data
{
	physx::PxVec3 pos = physx::PxVec3(0); //la posicion por defecto es el origen
	physx::PxVec4 color = physx::PxVec4(1, 1, 1, 1); //el color por defecto es blanco
	physx::PxVec3 vel = physx::PxVec3(0); //por defecto no hay velocidad
	physx::PxVec3 acc = physx::PxVec3(0, -9.8, 0); //la aceleracion por defecto es la gravedad
	double damping = 0.99; 
	Entity::IntegrateMode tipo = Entity::EULER;
	float masa = 1.0f;
	double vida = 5.0;
};

