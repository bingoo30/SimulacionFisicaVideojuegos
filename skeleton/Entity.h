#pragma once
#include <PxPhysicsAPI.h>
#include <memory>
#include "RenderUtils.hpp"
class Entity
{
public:
	Entity(const physx::PxVec3& p, 
		   const physx::PxVec4& c, 
		   double m,
		   physx::PxShape* s, 
		   double v, 
		   double lt);
	virtual ~Entity();
	virtual void update(double t);
	bool isDead() const { return (lifetime > 0.0 && age >= lifetime); };



#pragma region getters
	RenderItem* getRenderItem() { return renderItem.get(); }; 
	const physx::PxTransform getTransform() const { return transform; };
	const physx::PxVec4& getColor() const { return color; };
	double getMass() const { return mass; };
	physx::PxShape* getShape() const { return shape; };
	double getVol() const { return volume; };
	double getLifeTime() const { return lifetime; };
#pragma endregion

protected:
#pragma region atributos protegidos

	//componente transform
	physx::PxTransform transform;
	// color
	physx::PxVec4 color;
	//masa
	double mass;
	//forma
	physx::PxShape* shape;
	//volumen de la particula (solo se usa para crear particulas hijas)
	double volume;
	//para quitarlo del vector si lleva mucho tiempo en la escena
	//segundos de vida 
	double lifetime; 


	//tiempo acumulado
	double age;
	//render item
	std::unique_ptr<RenderItem> renderItem;
#pragma endregion
#pragma region metodos protegidos (auxiliares)
	//metodo virtual puro para la integracion de la entidad
	virtual void integrate(double t) = 0;
	//actualizacion del timer
	void update_lifetime(double t);
	//derregistrar el render item actual, si existe
	void derregister_renderItem();
	//crear un render item nuevo. Si ya existe, elimina el anterior
	void create_renderItem();
	//comprobar si tenemos un render item valido
	bool isValidRenderItem() const;
#pragma endregion
private:
#pragma region atributos privados
	//flag para saber si una entidad tiene render item o no
	bool renderItemRegisted;
#pragma endregion
};

