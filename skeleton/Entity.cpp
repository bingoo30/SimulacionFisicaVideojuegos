#include "Entity.h"

using namespace physx;
Entity::Entity(const physx::PxVec3& p, const physx::PxVec4& c, double m, physx::PxShape* s, double v, double lt, IntegrateMode md)
:transform(p), color(c), mass(m), shape(s), volume(v), lifetime(lt), mode(md)
{
}

Entity::~Entity()
{
	derregister_renderItem();
}
void Entity::derregister_renderItem()
{
	if (renderItem && renderItemRegisted) {
		DeregisterRenderItem(renderItem.get());
		renderItemRegisted = false;
	}
}
void Entity::create_renderItem()
{
	derregister_renderItem();

	renderItem = std::make_unique<RenderItem>(shape, &transform, color);
	renderItemRegisted = true;
}
void Entity::create_renderItem(const physx::PxRigidActor* actor)
{
	derregister_renderItem();
	renderItem = std::make_unique<RenderItem>(shape, actor, color);
	renderItemRegisted = true;
}
bool Entity::check_death() {
	//esta vivo si no tiene tiempo de vida o cuando ya se acabo su tiempo
	alive = lifetime == -1.0 || !(lifetime > 0.0 && age >= lifetime);
	return alive;
};
void Entity::update_lifetime(double t)
{
	age += t;
}

bool Entity::is_valid_renderItem() const
{
	return renderItem != nullptr && renderItemRegisted;
}

void Entity::setColor(const physx::PxVec4& c)
{
	color = c;
	if (renderItem && renderItemRegisted)
		renderItem->color = c;
}