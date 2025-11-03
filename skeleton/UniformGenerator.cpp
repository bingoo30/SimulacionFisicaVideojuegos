#include "UniformGenerator.h"
#include <memory>
#include "Scene.h"
using namespace std;
using namespace physx;

Particle_List UniformGenerator::generate_particles(const Particle_Data& model, const Particle_Deviation_Data& deviation,int n, physx::PxGeometryType::Enum geo) {
    Particle_List particles;

    //decidir si queremos generar exactamente n particulas nuevas
    int count = n;
    if (deviation.r_cant) count = (int)(std::round(n * random_fraction()));

    for (int i = 0; i < count; ++i) {
        //posicion con desviacion uniforme
        PxVec3 pos = model.pos + PxVec3(
            uniform_dev(deviation.ori.x),
            uniform_dev(deviation.ori.y),
            uniform_dev(deviation.ori.z)
        );

        //velocidad con desviacion uniforme
        PxVec3 vel = model.vel + PxVec3(
            uniform_dev(deviation.vel.x),
            uniform_dev(deviation.vel.y),
            uniform_dev(deviation.vel.z)
        );

        //masa y duracion
        double mass = model.mass + uniform_dev(deviation.mas);
        double life = model.life + uniform_dev(deviation.dur);

        //color aleatorio
        PxVec4 color = deviation.r_color ? colors[model.color_offset, model.color_offset + model.color_tam] : model.color;

        //crear particula y insertar a la lista
        auto g = create_geometry(geo, PxVec3(model.vol));
        PxShape* sh = CreateShape(*g);
        Particle* p = new Particle(pos, color, mass, CreateShape(physx::PxSphereGeometry(model.vol)), model.vol, life, vel, model.type);
        p->create_renderItem();
        particles.push_back(unique_ptr<Particle>(p));
        Scene::add_particle_to_registry(p);
    }

    return particles;
}
