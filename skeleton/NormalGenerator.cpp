#include "NormalGenerator.h"
using namespace std;
using namespace physx;

std::list<Particle*> NormalGenerator::generate_particles(const Particle_Data& model, const Particle_Deviation_Data& deviation, int n, physx::PxGeometryType::Enum geo) {
    std::list<Particle*> particles;

    int count = n;
    if (deviation.r_cant) count = (int)(round(n * random_fraction()));

    for (int i = 0; i < count; ++i) {
        //posicion con desviacion normal
        PxVec3 pos = model.pos + PxVec3(
            normal_dev(deviation.ori.x),
            normal_dev(deviation.ori.y),
            normal_dev(deviation.ori.z)
        );

        //velocidad con desviacion normal
        PxVec3 vel = model.vel + PxVec3(
            normal_dev(deviation.vel.x),
            normal_dev(deviation.vel.y),
            normal_dev(deviation.vel.z)
        );

        //masa y duracion
        double mass = model.masa + normal_dev(deviation.mas);
        double life = model.vida + normal_dev(deviation.dur);

        //color aleatorio
        PxVec4 color = deviation.r_color ? colors[random_color_index(model.color_offset, model.color_offset + model.color_tam)] : model.color;

        //crear particula y anadir a la lista
        PxGeometry g = create_geometry(geo, PxVec3(model.volumen, model.volumen, model.volumen));
        PxShape* sh = CreateShape(g);
        Particle* p = new Particle(pos, color, vel, model.acc, model.tipo, mass, life, sh);
        particles.push_back(p);
    }

    return particles;
}
