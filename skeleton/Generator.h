#pragma once
#include <random>
#include <list>
#include "Particle.h"
class Generator {
public:
#pragma region constructoras y destructoracxd
	Generator(const physx::PxVec3& o, const physx::PxVec3& v, double d, int n) {};
	//si uso esta constructora, hay que usar setters de los atributos
	Generator() {};
	virtual ~Generator() {};
#pragma endregion
#pragma region metodos
	virtual std::list<Particle*> generate_particles
	(const physx::PxVec3& pos_dev,
		const physx::PxVec3& vel_dev,
		double mas_dev,
		double dur_dev,
		bool rand_color, 
		bool rand_cant) = 0;
#pragma endregion
#pragma region getters y setters
	//origen
	const physx::PxVec3& getOrigin() const { return ori; };
	void setOrigin(const physx::PxVec3& newO) { ori = newO; };

	//velocidad
	const physx::PxVec3& getVelocity() const { return vel; };
	void setVelocity(const physx::PxVec3& newV) { vel = newV; };

	//duracion
	double getDuration() const { return duration; };
	void setDuration(double d) { duration = d; };

	//numero de particulas
	int getNParticles() const { return n_particles; };
	void setNParticles(int np) { n_particles = np; };
#pragma endregion
protected:
#pragma region atributos 
	physx::PxVec3 ori; //punto de origen de la particula generada
	physx::PxVec3 vel; //velocidad media de la particula generada
	double duration; //duracion de la particula generada
	int n_particles; //numero de particulas que voy a generar
	double mas; //masa
	physx::PxVec4 color; //color de la particula generada

	std::mt19937 _mt;
#pragma endregion
};
