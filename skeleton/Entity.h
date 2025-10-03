#pragma once
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
class Entity
{
public:
	//p =posicion inicial
	//c =color
	//s =forma de la geometria
	Entity(const physx::PxVec3& p = physx::PxVec3(0), const physx::PxVec4& c = physx::PxVec4(1), physx::PxShape* s=nullptr);
	virtual ~Entity();
protected:
	RenderItem* renderItem;
	physx::PxTransform* transform;	// posición
	physx::PxVec4 color;	// color
};

