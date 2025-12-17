#include "ButtonSystem.h"
#include "UniformGenerator.h"
#include <iostream>
#include <fstream>
using namespace physx;
using namespace std;
ButtonSystem::ButtonSystem(const Particle_Data& pd, const std::string& f):ParticleSystem(pd, Particle_Deviation_Data(), 1, PxGeometryType::eBOX), file(f) {

}
void ButtonSystem::init()
{
    spawn_acu = -1;
    add_generator(new UniformGenerator());
    std::ifstream f(file);
    if (!f.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << file << "\n";
        return;
    }

    int numButtons = 0;
    f >> numButtons; // Primera línea: número de botones

    // lo hago en una copia por si acaso
    const Particle_Data aux = model;
    for (int i = 0; i < numButtons; ++i) {
        f >> model.pos.x >> model.pos.y >> model.pos.z;
        int index;
        f >> index;
        model.color = colors[index];
        spawn(false, true);
    }

    // recupero mi model inicial
    model = aux;
    f.close();
}

bool ButtonSystem::check_out_of_limit(Particle* p) const
{
    return false;
}
