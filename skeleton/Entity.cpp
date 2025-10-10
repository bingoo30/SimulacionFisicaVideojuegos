#include "Entity.h"

using namespace physx;

Entity::Entity(const PxVec3& p, const PxVec4& c, PxShape* s, double lt, double m): transform(new PxTransform(p)), color(c), lifetime(lt), age(0.0), masa(m)
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
void Entity::update(double t) {
	integrate(t);
	update_lifetime(t);
}

void Entity::update_lifetime(double t)
{
	age += t;
}
