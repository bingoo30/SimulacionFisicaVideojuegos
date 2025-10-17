#include "NormalGenerator.h"
using namespace std;
using namespace physx;

std::list<Particle*>& NormalGenerator::generate_particles(const Particle_Data& model, const Particle_Deviation_Data& deviation, int n)
{
    mt19937 gen = _mt;
    normal_distribution<double> ur(-1.0, 1.0);
    uniform_int_distribution<int> ui(0, 7); // 8 colores (0–7)
    uniform_real_distribution<double> un(0.5, 1.0); //generador de la desviacion de cuantas particulas se va a generar

    std::list<Particle*> particles;

    //decidir si queremos meter aleatoriedad a la cantidad de generacio
    int count = n;
    if (deviation.r_cant) count = std::round(n * un(gen));

    for (int i = 0; i < count; ++i) {
        // posición con desviación
        PxVec3 pos = deviation.ori + PxVec3(ur(gen) * deviation.ori.x, ur(gen) * deviation.ori.y, ur(gen) * deviation.ori.z);

        // velocidad con desviación
        physx::PxVec3 vel_rand = model.vel + physx::PxVec3(
            ur(gen) * deviation.vel.x,
            ur(gen) * deviation.vel.y,
            ur(gen) * deviation.vel.z
        );

        // masa y duracion
        double mas_rand = model.masa + ur(gen) * deviation.mas;
        double dur_rand = model.vida + ur(gen) * deviation.dur;

        // color aleatorio
        physx::PxVec4 color_rand;
        if (deviation.r_color) color_rand = colors[ui(gen)];
        else color_rand = model.color;

        Particle* p = new Particle(
            pos,
            color_rand,
            vel_rand,
            model.acc,
            model.tipo,
            mas_rand,
            dur_rand,
            model.volumen
        );

        particles.push_back(p);
}
