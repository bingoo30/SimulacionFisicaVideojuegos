#pragma once
#include "Particle.h"
#include "Generator.h"
#include <list>
class ParticleSystem
{
public:
	ParticleSystem();
	ParticleSystem(int n, Generator* g);
	virtual ~ParticleSystem();

	Generator* getGenerator() { return gen; };
	void setGenerator(Generator* g) { gen = g; init(); };

	int getN() { return num; };
	void setN(int nn) { num = nn; };


	void spawn();
	void update(double dt);
protected:
#pragma region atributos
	Generator* gen;
	std::list<Particle*> particles_list;

	//numero de particulas que quiero spawnear
	int num;
#pragma endregion

#pragma region atributos personalizados
	//atributos que hay que modificar en la constructora de las subclases
	physx::PxVec3 pos_d;
	physx::PxVec3 vel_d;
	double mas_d;
	double dur_d;
	bool r_color;
	bool r_cant;
#pragma endregion

#pragma region metodos auxiliares
	void erase();
	//metodo para inicializar el generador, obligatorio a usarlo
	virtual void init() = 0;
#pragma endregion
};

