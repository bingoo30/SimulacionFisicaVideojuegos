#include "ForceRegistry.h"
#include "SceneManager.h"
ForceRegistry::ForceRegistry(): registries(),forces()
{
}
ForceRegistry::~ForceRegistry()
{
    
    registries.clear(); //solo limpia el vector
    for (auto f : forces) delete f;
    forces.clear();
}
void ForceRegistry::add_registry(Particle* particle, ForceGenerator* fg) {
    if (particle != nullptr && fg != nullptr) {
        registries.push_back({ particle, fg });
        //solo meter fuerzas que no son la gravedad, la gravedad se borra manualmente en cada escena
        if (fg != SceneManager::instance().getCurrScene()->getGravityGenerator()) {
            forces.push_back(fg);
        }
    }
}
void ForceRegistry::remove(Particle* particle, ForceGenerator* fg) {
    registries.erase(
        std::remove_if(registries.begin(), registries.end(),
            [&](const ForceReg& reg) {
                return reg.particle == particle && reg.fg == fg;
            }),
        registries.end());
}
void ForceRegistry::clear_particle(Particle* particle) {
    registries.erase(
        std::remove_if(registries.begin(), registries.end(),
            [&](const ForceReg& reg) {
                return reg.particle == particle;
            }),
        registries.end());
}


void ForceRegistry::update_forces() {
    for (auto& reg : registries) {
        if (reg.particle && reg.fg) {
            reg.particle->clean_force();
            reg.fg->update_force(reg.particle);
        }
    }
}