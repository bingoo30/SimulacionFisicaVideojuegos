#pragma once
#include "ParticleSystem.h"

class SparkParticleSystem : public ParticleSystem {
public:
    SparkParticleSystem(const Particle_Data& pd, const Particle_Deviation_Data& pdd, int n);
    virtual ~SparkParticleSystem() = default;
    bool check_out_of_limit(Particle* p) const override;
};


