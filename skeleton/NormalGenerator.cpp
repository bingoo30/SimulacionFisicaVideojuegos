#include "NormalGenerator.h"
#include "StructForEntities.h"
#include "Entity.h"
using namespace std;
using namespace physx;
std::list<Particle*> NormalGenerator::generate_particles(const PxVec3& pos_dev, const PxVec3& vel_dev,
    double mas_dev, double dur_dev, bool rand_color, bool rand_cant)
{
    mt19937 gen = _mt;
    normal_distribution<double> ur(-1.0, 1.0);
    uniform_int_distribution<int> ui(0, 7); // 8 colores (0–7)
    uniform_real_distribution<double> un(0.5, 1.0); //generador de la desviacion de cuantas particulas se va a generar

    std::list<Particle*> particles;

    //decidir si queremos meter aleatoriedad a la cantidad de generacio
    int count = n_particles;
    if (rand_cant) count = std::round(n_particles * un(gen)); 

    for (int i = 0; i < count; ++i) {
        // posición con desviación
        PxVec3 pos = ori + PxVec3(ur(gen) * pos_dev.x, ur(gen) * pos_dev.y, ur(gen) * pos_dev.z);

        // velocidad con desviación
        physx::PxVec3 vel_rand = vel + physx::PxVec3(
            ur(gen) * vel_dev.x,
            ur(gen) * vel_dev.y,
            ur(gen) * vel_dev.z
        );

        // masa y duracion
        double mas_rand = mas + ur(gen) * mas_dev;
        double dur_rand = duration + ur(gen) * dur_dev;

        // color aleatorio
        physx::PxVec4 color_rand;
        if (rand_color) color_rand = colors[ui(gen)];
        else color_rand = color;

        Particle* p = new Particle(
            pos,
            color_rand,
            vel_rand,
            CONST_GRAVITY,
            Entity::VERLET,
            mas_rand,
            dur_rand
        );

        particles.push_back(p);
    }

    return particles;
}
