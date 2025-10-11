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
	double volumen = 0.1;
};
struct Projectile_Data {
	physx::PxVec3 pos = physx::PxVec3(0);
	physx::PxVec4 color = physx::PxVec4(1, 1, 1, 1);
	double offset = 0.0;

	double vel_real;
	double vel_sim;
	physx::PxVec3 acc = physx::PxVec3(0, -9.8, 0);
	double damping = 0.99;
	float masa = 10.0f;
	double vida = 5.0;
	double volumen = 0.1;
};
//bala de una pistola
struct Pistol_Bullet_Data : public Projectile_Data {
	Pistol_Bullet_Data() {
		color = physx::PxVec4(0.8f, 0.6f, 0.2f, 1.0f); //bronce
		masa = 0.008f;                      
		vida = 2.5;  
		vel_real = 350.0;
		vel_sim = 100.0;
		offset = 1.0;
		volumen = 0.2;
	}
};
//canon
struct Cannon_Data : public Projectile_Data {
	Cannon_Data() {
		color = physx::PxVec4(1.0f, 0.4f, 0.0f, 1.0f); //naranja
		masa = 3.5f;
		vida = 5.0;
		vel_real = 1000.0;
		vel_sim = 500.0;
		offset = 5.0;
		volumen = 3.5;
	}
};

physx::PxVec3 CONST_GRAVITY = physx::PxVec3(0.0f, -9.8f, 0.0f);

//array de colores
physx::PxVec4 colors[8] = {
physx::PxVec4(1, 0, 0, 1), // rojo
physx::PxVec4(0, 1, 0, 1), // verde
physx::PxVec4(0, 0, 1, 1), // azul
physx::PxVec4(1, 1, 0, 1), // amarillo
physx::PxVec4(1, 0, 1, 1), // magenta
physx::PxVec4(0, 1, 1, 1),  // cian
physx::PxVec4(1, 1, 1, 1), // blanco
physx::PxVec4(0, 0, 0, 1), // negro
};
