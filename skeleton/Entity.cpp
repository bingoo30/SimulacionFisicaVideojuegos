#include "Entity.h"

using namespace physx;

Entity::Entity(const PxVec3& p, const PxVec4& c, PxShape* s): transform(new PxTransform(p)), color(c)
{
	// Inicializar RenderItem
	renderItem = new RenderItem(s, transform, color);
	RegisterRenderItem(renderItem);
}

Entity::~Entity()
{
	DeregisterRenderItem(renderItem);
	delete renderItem;
	delete transform;
}
