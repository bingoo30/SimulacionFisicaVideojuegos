#pragma once
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include "Entity.h"
#pragma region practica 1
//fichero auxiliar que guardan los structs(datos) necesarios necesarios para inicializar un objeto de la escena
struct Particle_Data
{
	physx::PxVec3 pos = physx::PxVec3(0); //la posicion por defecto es el origen
	physx::PxVec4 color = physx::PxVec4(1, 1, 1, 1); //el color por defecto es blanco
	physx::PxVec3 vel = physx::PxVec3(0); //por defecto no hay velocidad
	Entity::IntegrateMode type = Entity::EULER;
	float mass = 0.0f;
	double life = 5.0;
	double vol = 1.0;
	int color_offset = 0;
	int color_tam =8;
};

struct Projectile_Data {
	physx::PxVec3 pos = physx::PxVec3(0);
	physx::PxVec4 color = physx::PxVec4(1, 1, 1, 1);
	double offset = 0.0;

	double vel_real;
	double vel_sim;
	physx::PxVec3 acc = physx::PxVec3(0, -10.0, 0);
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
		volumen = 7.5;
	}
};

inline physx::PxVec3 CONST_GRAVITY = physx::PxVec3(0.0f, -10.0f, 0.0f);
#pragma endregion

#pragma region practica 2
//array de colores
inline const physx::PxVec4 colors[12] = {
	physx::PxVec4(1, 0, 0, 1), // rojo
	physx::PxVec4(0, 1, 0, 1), // verde
	physx::PxVec4(0, 0, 1, 1), // azul
	physx::PxVec4(1, 1, 0, 1), // amarillo
	physx::PxVec4(1, 0, 1, 1), // magenta
	physx::PxVec4(0, 1, 1, 1), // cian
	physx::PxVec4(0, 0, 0, 1), // negro
	physx::PxVec4(1, 1, 1, 1), // blanco
	//a partir de aqui es el color del fuego
	physx::PxVec4(0.5f, 0.0f, 0.0f, 1.0f),  // rojo oscuro
	physx::PxVec4(0.75f, 0.05f, 0.0f, 1.0f), // rojo intenso
	physx::PxVec4(0.9f, 0.1f, 0.0f, 1.0f),   // rojo brillante
	physx::PxVec4(1.0f, 0.2f, 0.1f, 1.0f)    // rojo claro
};

struct Particle_Deviation_Data {
	physx::PxVec3 ori;
	physx::PxVec3 vel;
	double mas;
	double dur;
	double valid_box;
	bool r_color = false;
	bool r_cant = false;
};

struct Fire_Particle_Data:public Particle_Data {
	Fire_Particle_Data() {
		color = physx::PxVec4(1, 0, 0, 1); //color rojo
		vel = physx::PxVec3(0, 20.0, 0); //hacia arriba
		type = Entity::EULER_SEMIIMPLICIT;
		life = 1.0;
		vol = 0.85;
		color_offset = 8;
		color_tam = 4;
	}
};
struct Fire_Deviation_Data : public Particle_Deviation_Data {
	Fire_Deviation_Data() {
		ori = physx::PxVec3(1.5f, 0.0f, 1.5f);
		vel = physx::PxVec3(0.75f, 2.0f, 0.75f);
		mas = 0.02;
		dur = 0.5;
		r_color = true;
		r_cant = true;
		valid_box = 5.0;
	}
};

struct Rain_Particle_Data : public Particle_Data {
	Rain_Particle_Data() {
		color = physx::PxVec4(0.0f, 0.0f, 0.8f, 1.0f); // azul claro tipico de la lluvia
		vel = physx::PxVec3(0.0f, -10.0f, 0.0f);       // caida rapida hacia abajo
		type = Entity::VERLET;            // integracion estable para velocidad alta
		life = 3.0;                                    // suficiente para recorrer la zona
		vol = 0.35f;                               // gotas finas
		mass = 0.01f;                                  // ligera
	}
};
struct Rain_Deviation_Data :public Particle_Deviation_Data {
	Rain_Deviation_Data() {
		ori = physx::PxVec3(50.0f, 0.0f, 50.0f);
		vel = physx::PxVec3(1.0f, 2.0f, 1.0f);
		mas = 0.002; 
		dur = 0.5; 
		valid_box = 10.0; 
		r_color = false;
		r_cant = true; 
	}
};

struct Firework_Rocket_Data : public Particle_Data {
	Firework_Rocket_Data() {
		color = physx::PxVec4(1.0f, 1.0f, 1.0f, 1.0f);  // blanco brillante
		vel = physx::PxVec3(0, 40.0f, 0);               // hacia arriba
		mass = 0.5f;
		life = 3.0;                                     // explota a los 3 segundos
		vol = 1.5f;
		type = Entity::EULER_SEMIIMPLICIT;
	}
};

struct Firework_Explosion_Deviation : public Particle_Deviation_Data {
	Firework_Explosion_Deviation() {
		vel = physx::PxVec3(30.0f, 20.0f, 30.0f);       // dispersion radial
		ori = physx::PxVec3(0.0f);
		mas = 0.1;
		dur = 0.5;
		r_color = true;                                 // colores aleatorios
		r_cant = false;
		valid_box = 5.0;
	}
};
#pragma endregion

