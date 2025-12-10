#include "NormalGenerator.h"
#include "SceneManager.h"
#include "Scene.h"
using namespace std;
using namespace physx;

Particle_List NormalGenerator::generate_particles(const Particle_Data& model, const Particle_Deviation_Data& deviation, int n, physx::PxGeometryType::Enum geo, physx::PxMaterial* _mat) {
    Particle_List particles;

    int count = n;
    if (deviation.r_cant) count = (int)(round(n * random_fraction()));

    for (int i = 0; i < count; ++i) {
        //posicion con desviacion normal
        PxVec3 pos = give_a_new_vec3(model.pos, deviation.ori, false);

        //velocidad con desviacion normal
        PxVec3 vel = give_a_new_vec3(model.vel, deviation.vel, false);

        //masa y duracion
        double mass = give_a_new_double(model.mass, deviation.mas, false);
        double life = give_a_new_double(model.lifetime, deviation.dur, false);

        //color aleatorio
        PxVec4 color = deviation.r_color ? colors[random_color_index(model.color_offset, model.color_offset + model.color_tam)] : model.color;

        //crear particula y anadir a la lista
        auto g = create_geometry(geo, PxVec3(model.vol));
        PxShape* sh = CreateShape(*g);
        Particle* p = new Particle(pos, color, mass, CreateShape(physx::PxSphereGeometry(model.vol)), model.vol, life, vel, SEMI_IMPLICIT_EULER, model.density);
        p->create_renderItem();
        particles.push_back(p);
    }
    return particles;
}
