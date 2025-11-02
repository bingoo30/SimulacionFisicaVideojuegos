#include "Entity.h"

using namespace physx;





Entity::Entity(const physx::PxVec3& p, const physx::PxVec4& c, double m, PxShape* sh, double v, double lt)
:transform(PxTransform(p)), color(c), mass(m), shape(sh), volume(v),lifetime(lt),
renderItem(nullptr), age(0.0), renderItemRegisted(false)
{
}

Entity::~Entity()
{
    derregister_renderItem();
}
void Entity::update(double t) {
	integrate(t);
	update_lifetime(t);
}

void Entity::update_lifetime(double t)
{
	age += t;
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
    RegisterRenderItem(renderItem.get());
    renderItemRegisted = true;
}
bool Entity::isValidRenderItem() const
{
    return renderItem != nullptr && renderItemRegisted;
}