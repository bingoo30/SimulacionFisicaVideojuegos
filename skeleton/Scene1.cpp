#include "Scene1.h"
#include "Entity.h"
Scene1::Scene1(): Scene()
{
}

void Scene1::init()
{

}

void Scene1::update(double t)
{
    // integrar y comprobar tiempo de vida
    for (auto it = gObjs.begin(); it != gObjs.end();) {
        Entity* e = *it;
        if (e != nullptr) {
            e->update(t);

            if (e->isDead()) {
                DeregisterRenderItem(e->getRenderItem());
                delete e;
                it = gObjs.erase(it);
            }
            else {
                ++it;
            }
        }
    }
}
