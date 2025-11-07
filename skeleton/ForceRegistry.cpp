#include "ForceRegistry.h"
#include "SceneManager.h"
ForceRegistry::ForceRegistry(): registries()
{
}
ForceRegistry::~ForceRegistry()
{
    
    registries.clear(); //solo limpia el vector
}
void ForceRegistry::add_registry(Particle* particle, ForceGenerator* fg) {
    if (particle != nullptr && fg != nullptr) {
        // Evitar duplicados, que sino la fuerza de gravedad se va aumentando
        auto it = std::find_if(registries.begin(), registries.end(),
            [&](const ForceReg& reg) {
                return reg.particle == particle && reg.fg == fg;
            });

        if (it == registries.end()) {
            registries.push_back({ particle, fg });
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


void ForceRegistry::update_forces(double dt) {
    registries.erase(
        std::remove_if(registries.begin(), registries.end(),
            [](const ForceReg& reg) {
                // quitar una particula está muerta o invalida
                return reg.particle == nullptr || !reg.particle->is_alive();
            }),
        registries.end());

    for (auto& reg : registries) {
        if (reg.particle && reg.fg) {
            reg.fg->update_force(reg.particle, dt);
        }
    }
}
