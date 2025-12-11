#pragma once
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include <memory>
enum IntegrateMode{EULER, SEMI_IMPLICIT_EULER, VERLET, NONE};
class Entity
{
public:
#pragma region constructoras y destructora
	Entity(const physx::PxVec3& p,
		const physx::PxVec4& c,
		double m,
		physx::PxShape* s,
		double v,
		double lt,
		IntegrateMode md);
	virtual ~Entity();
#pragma endregion
#pragma region metodos publicos
	//derregistrar el render item actual, si existe
	void derregister_renderItem();
	//crear un render item nuevo. Si ya existe, elimina el anterior
	virtual void create_renderItem() = 0;
	//update
	virtual void update(double dt) =0;
	//comprueba si una entidad se ha pasado de su tiempo de vida((devuelve true), en caso contrario, devuelve false)
	bool check_death();
	//comprobar si tenemos un render item valido
	bool is_valid_renderItem() const;
	virtual void on_death() {};
#pragma endregion
#pragma region getters
	RenderItem* getRenderItem() { return renderItem.get(); };
	const physx::PxTransform getTransform() const { return transform; };
	const physx::PxVec4& getColor() const { return color; };
	virtual double getMass() const { return mass; };
	physx::PxShape* getShape() const { return shape; };
	double getVol() const { return volume; };
	double getLifeTime() const { return lifetime; };
	bool is_alive() const { return alive; };
#pragma endregion
	void setColor(const physx::PxVec4& c);
protected:
#pragma region atributos protegidos
	//atributos que inicializo

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
	//tipo de integracion que voy a usar
	IntegrateMode mode;

	//atributos que se inicializan con un valor
	//tiempo acumulado
	double age=0.0;
	//render item
	std::unique_ptr<RenderItem> renderItem = nullptr;
	//flag para marcar si una entidad esta viva o muerta
	bool alive = true;
#pragma endregion
#pragma region metodos protegidos (auxiliares)
	//metodo virtual puro para la integracion de la entidad
	virtual void integrate(double t) = 0;
	//actualizacion del timer
	void update_lifetime(double t);
#pragma endregion
#pragma region atributos privados
	//flag para saber si una entidad tiene render item o no
	bool renderItemRegisted = false;
#pragma endregion
};

