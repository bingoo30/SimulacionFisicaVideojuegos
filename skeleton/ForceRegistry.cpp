#include "ForceRegistry.h"
void ForceRegistry::addRegistry(Particle* particle, ForceGenerator* fg) {
    if (particle && fg)
        registries.push_back({ particle, fg });
}
void ForceRegistry::remove(Particle* particle, ForceGenerator* fg) {
    registries.erase(
        std::remove_if(registries.begin(), registries.end(),
            [&](const ForceReg& reg) {
                return reg.particle == particle && reg.fg == fg;
            }),
        registries.end());
}
void ForceRegistry::clearParticle(Particle* particle) {
    registries.erase(
        std::remove_if(registries.begin(), registries.end(),
            [&](const ForceReg& reg) {
                return reg.particle == particle;
            }),
        registries.end());
}

void ForceRegistry::clear() {
    registries.clear();
}

void ForceRegistry::updateForces() {
    for (auto& reg : registries) {
        if (reg.particle && reg.fg) {
            reg.particle->clean_force();
            reg.fg->update_force(reg.particle);
        }
    }
}