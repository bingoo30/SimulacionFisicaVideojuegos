#include "Entity.h"

using namespace physx;

Entity::Entity(
    const PxVec3& p,
    const PxVec4& c,
    PxShape* s,
    double vol,
    double lt,
    double m)
    : transform(new physx::PxTransform(p)),
    color(c), 
    masa(m),
    lifetime(lt),
    age(0.0),
    volume(vol),
    renderItem(new RenderItem(s, transform, color))
{
}

Entity::~Entity()
{
	DeregisterRenderItem(renderItem);
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
