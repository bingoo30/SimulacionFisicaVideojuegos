#pragma once
#include "ParticleSystem.h"
class FireParticleSystem :
    public ParticleSystem
{
public:
    FireParticleSystem(const Particle_Data& pd, const Particle_Deviation_Data& pdd, int n);
protected:
    bool check_out_of_limit(Particle* p) const override;
};

