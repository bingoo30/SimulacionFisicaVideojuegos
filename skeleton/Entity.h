#pragma once
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
class Entity
{
public:
	enum IntegrateMode {
		EULER,
		EULER_SEMIIMPLICIT,
		VERLET
	};
	enum ParticleType {
		NORMAL,
		FIREWORK_ROCKET,
		FIREWORK_SPARK
	};
	//p =posicion inicial
	//c =color
	//s =forma de la geometria
	Entity(const physx::PxVec3& p = physx::PxVec3(0), const physx::PxVec4& c = physx::PxVec4(1), physx::PxShape* s=nullptr, double vol=1.0, double lt = -1.0, double m = 0.0);
	virtual ~Entity();
	virtual void update(double t);
	RenderItem* getRenderItem() { return renderItem; };
	bool isDead() const { return (lifetime > 0.0 && age >= lifetime); };

	double getMass() const { return masa; };
	double getLifeTime() const { return lifetime; };
	double getVol() const { return volume; };
protected:
	virtual void integrate(double t) = 0;
	void update_lifetime(double t);
	RenderItem* renderItem;
	physx::PxTransform* transform;	// posición
	physx::PxVec4 color;	// color
	double masa;  //masa

	//para quitarlo del vector si lleva mucho tiempo en la escena
	double lifetime;  // segundos de vida (-1 = infinito)
	double age;       // tiempo acumulado
	double volume; //volumen de la particula (solo se usa para crear particulas hijas)
};

