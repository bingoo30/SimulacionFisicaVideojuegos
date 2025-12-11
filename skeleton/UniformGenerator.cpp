#include "UniformGenerator.h"
#include "SceneManager.h"
using namespace std;
using namespace physx;

Particle_List UniformGenerator::generate_particles(const Particle_Data& model, const Particle_Deviation_Data& deviation,int n, physx::PxGeometryType::Enum geo, physx::PxMaterial* _mat) {
    Particle_List particles;

    //decidir si queremos generar exactamente n particulas nuevas
    int count = n;
    if (deviation.r_cant) count = (int)(round(n * random_fraction()));

    for (int i = 0; i < count; ++i) {
        //posicion con desviacion uniforme
        PxVec3 pos = give_a_new_vec3(model.pos, deviation.ori);

        //velocidad con desviacion uniforme
        PxVec3 vel = give_a_new_vec3(model.vel, deviation.vel);

        //masa y duracion
        double mass = give_a_new_double(model.mass, deviation.mas);
        double life = give_a_new_double(model.lifetime, deviation.dur);

        //color aleatorio
        PxVec4 color = deviation.r_color ? colors[model.color_offset, model.color_offset + model.color_tam] : model.color;

        //crear particula y insertar a la lista
        auto g = create_geometry(geo, PxVec3(model.vol, model.vol, model.vol));
        PxShape* sh = CreateShape(*g);
        Particle* p = new Particle(pos, color, mass, sh, model.vol, life, vel, SEMI_IMPLICIT_EULER, model.density);
        p->create_renderItem();
        particles.push_back(p);
    }

    return particles;
}
