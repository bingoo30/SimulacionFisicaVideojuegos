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
	IntegrateMode mode = IntegrateMode::SEMI_IMPLICIT_EULER;
	double mass = 0.1;
	double lifetime = 5.0;
	double vol = 1.0;
	int color_offset = 0;
	int color_tam =8;
	double density = 1.225;

};

struct Projectile_Data {
	physx::PxVec3 pos = physx::PxVec3(0);
	physx::PxVec4 color = physx::PxVec4(1, 1, 1, 1);
	double offset = 0.0;
	double vel_real;
	double vel_sim;
	double mass = 10.0;
	double lifetime = 5.0;
	double vol = 0.1;
	double density = 1.225;
	IntegrateMode mode = IntegrateMode::SEMI_IMPLICIT_EULER;
};
//bala de una pistola
struct Pistol_Bullet_Data : public Projectile_Data {
	Pistol_Bullet_Data() {
		color = physx::PxVec4(0.8f, 0.6f, 0.2f, 1.0f); //bronce
		mass = 0.008f;
		lifetime = 2.5;
		vel_real = 350.0;
		vel_sim = 100.0;
		offset = 1.0;
		vol = 0.2;
		density = 10500.0;
	}
};
//canon
struct Cannon_Data : public Projectile_Data {
	Cannon_Data() {
		color = physx::PxVec4(1.0f, 0.4f, 0.0f, 1.0f); //naranja
		mass = 15.6f;
		lifetime = 5.0;
		vel_real = 1000.0;
		vel_sim = 300.0;
		offset = 5.0;
		vol = 7.5;
		density = 7850.0;
	}
};

inline physx::PxVec3 CONST_GRAVITY = physx::PxVec3(0.0f, -10.0f, 0.0f);
#pragma endregion

#pragma region practica 2
//array de colores
inline const physx::PxVec4 colors[20] = {
	physx::PxVec4(1, 0, 0, 1), // rojo
	physx::PxVec4(0, 1, 0, 1), // verde
	physx::PxVec4(0, 0, 1, 1), // azul
	physx::PxVec4(1, 1, 0, 1), // amarillo
	physx::PxVec4(1, 0, 1, 1), // magenta
	physx::PxVec4(0, 1, 1, 1), // cian
	physx::PxVec4(0, 0, 0, 1), // negro
	physx::PxVec4(1, 1, 1, 1), // blanco
	//a partir de aqui es una escala de rojos
	physx::PxVec4(0.5f, 0.0f, 0.0f, 1.0f),  // rojo oscuro
	physx::PxVec4(0.75f, 0.05f, 0.0f, 1.0f), // rojo intenso
	physx::PxVec4(0.9f, 0.1f, 0.0f, 1.0f),   // rojo brillante
	physx::PxVec4(1.0f, 0.2f, 0.1f, 1.0f),    // rojo claro
	//a partir de aqui es una escala de verdes
	physx::PxVec4(0.0f, 0.3f, 0.0f, 1.0f),   // verde oscuro (bosque)
	physx::PxVec4(0.0f, 0.5f, 0.0f, 1.0f),   // verde medio (hierba)
	physx::PxVec4(0.2f, 0.7f, 0.2f, 1.0f),   // verde brillante (hoja fresca)
	physx::PxVec4(0.5f, 1.0f, 0.5f, 1.0f),   // verde claro (luminoso)
	//a partir de aqui es una escala de morados
	physx::PxVec4(0.2f, 0.0f, 0.3f, 1.0f),   // púrpura oscuro (violeta profundo)
	physx::PxVec4(0.4f, 0.0f, 0.6f, 1.0f),   // morado intenso
	physx::PxVec4(0.6f, 0.2f, 0.8f, 1.0f),   // lavanda brillante
	physx::PxVec4(0.8f, 0.5f, 1.0f, 1.0f)   // violeta claro (pastel)
};

struct Particle_Deviation_Data {
	physx::PxVec3 ori;
	physx::PxVec3 vel;
	double mas;
	double dur;
	physx::PxVec3 valid_box;
	bool r_color = false;
	bool r_cant = false;
};

struct Fire_Particle_Data:public Particle_Data {
	Fire_Particle_Data() {
		color = physx::PxVec4(1, 0, 0, 1); //color rojo
		vel = physx::PxVec3(0, 20.0, 0); //hacia arriba
		mode = IntegrateMode::SEMI_IMPLICIT_EULER;
		lifetime = 0.75;
		vol = 0.55;
		color_offset = 8;
		color_tam = 4;
		density = 0.75; //datos reales
		mass = 0.0025;
	}
};
struct Fire_Deviation_Data : public Particle_Deviation_Data {
	Fire_Deviation_Data() {
		ori = physx::PxVec3(1.5f, 0.0f, 1.5f);
		vel = physx::PxVec3(0.75f, 5.0f, 0.75f);
		mas = 0.0005;
		dur = 0.5;
		r_color = true;
		r_cant = true;
		valid_box = physx::PxVec3(60, 40, 30);
		
	}
};

struct Rain_Particle_Data : public Particle_Data {
	Rain_Particle_Data() {
		color = physx::PxVec4(0.0f, 0.0f, 0.8f, 1.0f); // azul claro tipico de la lluvia
		vel = physx::PxVec3(0.0f, -5.0f, 0.0f);       // caida rapida hacia abajo
		mode = IntegrateMode::VERLET;            // integracion estable para velocidad alta
		lifetime = 10.0;                                    // suficiente para recorrer la zona
		vol = 0.2f;                               // gotas finas
		mass = 0.00042;                                  // ligera
		density = 1000.0;
	}
};
struct Rain_Deviation_Data :public Particle_Deviation_Data {
	Rain_Deviation_Data() {
		ori = physx::PxVec3(80.0f, 0.0f, 80.0f);
		vel = physx::PxVec3(1.0f, 2.0f, 1.0f);
		mas = 0.0; 
		dur = 0.5; 
		r_color = false;
		r_cant = true; 
	}
};

struct Firework_Particle_Data : public Particle_Data {
	Firework_Particle_Data() {
		color = physx::PxVec4(1, 0, 0, 1); //color rojo
		vel = physx::PxVec3(0, 20.0, 0); //hacia arriba
		mode = IntegrateMode::VERLET;
		lifetime = 0.75;
		vol = 0.55;
		mass = 0.0025;
	}
};
struct Firework_Deviation_Data :public Particle_Deviation_Data {
	Firework_Deviation_Data() {
		vel = physx::PxVec3(1.0f, 2.0f, 1.0f);
		mas = 0.0;
		dur = 0.005;
		r_color = false;
		r_cant = false;
	}
};
#pragma endregion

#pragma region practica 3
#ifndef PI
#define PI 3.14159265358979323846
#endif

struct Wind_Data {
	physx::PxVec3 center = physx::PxVec3(0.0, 0.0, 0.0);
	//velocidad del viento
    //viento moderado: 5–15 m/s; fuerte: 20–30 m/s
	physx::PxVec3 vel = physx::PxVec3(10.0, 0.0, 0.0);
	//radio del area de influencia
	double area = 200.0;
	//coeficiente de rozamiento del aire
	double k1 = 0.1;
	//densidad 1.225 (en principio no cambia, por eso no lo pongo aqui)
	//coeficiente de arrastre aerodinámico
	//esferas aprox 0.47, cubos aprox 1.05, cuerpos aerodinamicos aprox 0.1–0.3
	double dragCoef = 0.47;
};

struct Whirlwind_Data: public Wind_Data {
	Whirlwind_Data() {
	}
	double K = 1.5;
};
struct Explosion_Data {
	physx::PxVec3 center = physx::PxVec3(0.0, 0.0, 0.0);
	double radius = 20.0;
	double K = 8000.0;
	double tau = 1.0;
};
#pragma endregion

#pragma region proyecto
struct Platform_Data:public Particle_Data {
	Platform_Data() {
		color_offset = 12;
		color = physx::PxVec4(0.5f, 1.0f, 0.0f, 1.0f);
		mode = IntegrateMode::NONE;
		mass = 10.0;
		lifetime = -1;
		color_tam = 4;
		density = 1.225;
	}
	double volx =1.0;
	double voly=1.0;
	double volz=1.0;
};
#pragma endregion

