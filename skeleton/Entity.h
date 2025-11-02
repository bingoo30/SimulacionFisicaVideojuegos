#pragma once
#include <PxPhysicsAPI.h>
#include <memory>
#include "RenderUtils.hpp"
class Entity
{
public:
	Entity(const physx::PxVec3& p = physx::PxVec3(0), 
		   const physx::PxVec4& c = physx::PxVec4(1), 
		   physx::PxShape* s=nullptr, 
		   double vol=1.0, 
		   double lt = -1.0, 
		   double m = 0.0);
	Entity(const physx::PxVec3& p = physx::PxVec3(0),
		const physx::PxVec4& c = physx::PxVec4(1),
		physx::PxShape* s = nullptr,
		double vol = 1.0,
		double lt = -1.0,
		double m = 0.0,
		bool create);
	virtual ~Entity();
	virtual void update(double t);
	RenderItem* getRenderItem() { return renderItem.get(); };
	bool isDead() const { return (lifetime > 0.0 && age >= lifetime); };

	double getMass() const { return mass; };
	double getLifeTime() const { return lifetime; };
	double getVol() const { return volume; };
	physx::PxShape* getShape() const { return shape; };
protected:
	virtual void integrate(double t) = 0;
	void update_lifetime(double t);
	void derregisterRenderItem();
	void create_renderItem();
	bool isValidRenderItem() const;
	std::unique_ptr<RenderItem> renderItem;
	physx::PxTransform transform;
	physx::PxVec4 color;	// color
	double mass;  //masa
	physx::PxShape* shape;

	//para quitarlo del vector si lleva mucho tiempo en la escena
	double lifetime;  // segundos de vida (-1 = infinito)
	double age;       // tiempo acumulado
	double volume; //volumen de la particula (solo se usa para crear particulas hijas)
private:
	bool renderItemRegisted;
};

