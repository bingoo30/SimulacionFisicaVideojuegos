#pragma once
#include "ParticleSystem.h"
class FireParticleSystem :
    public ParticleSystem
{
public:
    FireParticleSystem(const Particle_Data& pd, const Particle_Deviation_Data& pdd, int n);
    virtual ~FireParticleSystem();
};

