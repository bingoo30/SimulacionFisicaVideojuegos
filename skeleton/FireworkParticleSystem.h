#pragma once
#include "ParticleSystem.h"
class FireworkParticleSystem : public ParticleSystem
{
public:
    FireworkParticleSystem(const Particle_Data& pd, const Particle_Deviation_Data& pdd, int n);
protected:
    bool check_out_of_limit(Particle* p) const override;
};

