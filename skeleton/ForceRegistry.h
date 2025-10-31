#pragma once
#include <vector>
#include <algorithm>
#include "Particle.h"
#include "ForceGenerator.h"

class ForceRegistry {
public:
    void addRegistry(Particle* particle, ForceGenerator* fg);
    void remove(Particle* particle, ForceGenerator* fg);
    void clearParticle(Particle* particle);
    void clear();
    void updateForces();
protected:
    struct ForceReg {
        Particle* particle;
        ForceGenerator* fg;
    };

    std::vector<ForceReg> registries;

};