#include "FireworkGenerator.h"
#include "ParticleSystem.h"
#include <memory>
using namespace std;
using namespace physx;
Particle_List FireworkGenerator::generate_particles(
    const Particle_Data& model,
    const Particle_Deviation_Data& deviation,
    int n,
    physx::PxGeometryType::Enum geo)
{
    Particle_List particles;

    // posición con desviación normal
    PxVec3 pos = model.pos + PxVec3(
        normal_dev(deviation.ori.x),
        normal_dev(deviation.ori.y),
        normal_dev(deviation.ori.z)
    );

    // velocidad con desviación normal
    PxVec3 vel = model.vel + PxVec3(
        normal_dev(deviation.vel.x),
        normal_dev(deviation.vel.y),
        normal_dev(deviation.vel.z)
    );

    // masa y duración
    double mass = model.mass + normal_dev(deviation.mas);
    double life = model.life + normal_dev(deviation.dur);

    // color
    PxVec4 color = deviation.r_color
        ? colors[random_color_index(model.color_offset, model.color_offset + model.color_tam)]
        : model.color;

    // crear geometría
    auto g = create_geometry(geo, PxVec3(model.vol, model.vol, model.vol));
    PxShape* sh = CreateShape(*g);

    // crear partícula principal (cohete)
    Particle* rocket = new Particle(pos, color, vel, model.type, mass, life, sh, model.vol);
    rocket->setParticleType(Entity::FIREWORK_ROCKET);
    // añadir callback para explosión
    rocket->set_on_death([this, n, model, deviation, geo](ParticleSystem* sys, const Particle* parent) {

        int count = n;
        if (deviation.r_cant)
            count = (int)(round(n * random_fraction()));

        for (int i = 0; i < count; ++i)
        {
            // color aleatorio
            PxVec4 color = deviation.r_color
                ? colors[random_color_index(model.color_offset, model.color_offset + model.color_tam)]
                : model.color;

            // velocidad aleatoria
            PxVec3 spark_vel(
                10.0f * uniform_dev(1.0f),
                10.0f * uniform_dev(1.0f),
                10.0f * uniform_dev(1.0f)
            );

            // masa y vida propias de los sparks
            double mass = parent->getMass() * 0.2 + normal_dev(deviation.mas);
            double life = 0.5 + normal_dev(deviation.dur);
            double vol = parent->getVol() * 0.75;

            auto g = create_geometry(geo, PxVec3(vol, vol, vol));
            PxShape* sh = CreateShape(*g);

            Particle* spark = new Particle(parent->getPosition(), color, spark_vel, parent->getType(), mass, life, sh, vol);
            spark->setParticleType(Entity::FIREWORK_SPARK);
            sys->add_particle(spark);
        }
        });

    // agregar partícula al listado
    particles.push_back(std::unique_ptr<Particle>(rocket));

    return particles;
}


