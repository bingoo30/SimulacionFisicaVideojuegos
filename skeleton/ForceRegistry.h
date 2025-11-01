#pragma once
#include <vector>
#include <algorithm>
#include "Particle.h"
#include "ForceGenerator.h"

class ForceRegistry {
public:
    ForceRegistry();
    virtual ~ForceRegistry();
    void add_registry(Particle* particle, ForceGenerator* fg);
    void remove(Particle* particle, ForceGenerator* fg);
    void clear_particle(Particle* particle);
    void update_forces();
protected:
    struct ForceReg {
        Particle* particle;
        ForceGenerator* fg;
    };

    std::vector<ForceReg> registries;

};