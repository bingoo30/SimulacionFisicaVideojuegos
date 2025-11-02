#include "Entity.h"

using namespace physx;

Entity::Entity(
    const PxVec3& p,
    const PxVec4& c,
    PxShape* s,
    double vol,
    double lt,
    double m)
    : transform(PxTransform(p)),
    color(c), 
    mass(m),
    shape(s),
    lifetime(lt),
    age(0.0),
    volume(vol),
    renderItem(nullptr),
    renderItemRegisted(false)
{
}

Entity::Entity(const physx::PxVec3& p, const physx::PxVec4& c, physx::PxShape* s, double vol, double lt, double m, bool create)
    : transform(PxTransform(p)),
    color(c),
    mass(m),
    shape(s),
    lifetime(lt),
    age(0.0),
    volume(vol),
    renderItem(nullptr),
    renderItemRegisted(false)
{
    if (create) create_renderItem();
}

Entity::~Entity()
{
    derregisterRenderItem();
}
void Entity::update(double t) {
	integrate(t);
	update_lifetime(t);
}

void Entity::update_lifetime(double t)
{
	age += t;
}

void Entity::derregisterRenderItem()
{
    if (renderItem && renderItemRegisted) {
        DeregisterRenderItem(renderItem.get());
        renderItemRegisted = false;
    }
}
void Entity::create_renderItem()
{
    derregisterRenderItem();

    renderItem = std::make_unique<RenderItem>(shape, &transform, color);
    RegisterRenderItem(renderItem.get());
    renderItemRegisted = true;
}
bool Entity::isValidRenderItem() const
{
    return renderItem != nullptr && renderItemRegisted;
}