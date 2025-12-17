#pragma once
#include "ParticleSystem.h"
#include <string>
class ButtonSystem :
    public ParticleSystem
{
public:
    ButtonSystem(const Particle_Data& pd, const std::string& f);
    void init() override;
protected:
    bool check_out_of_limit(Particle* p) const override;
    std::string file;
};

