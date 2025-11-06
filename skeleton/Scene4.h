#pragma once
#include "Scene.h"
#include "WhirlwindForceGenerator.h"
#include "ExplosionForceGenerator.h"
class Scene4 :
    public Scene
{
public:
    Scene4()= default;
    void init() override;
    Particle* create_particle(const Particle_Data& pd) override;
    Projectile* create_projectile(const Projectile_Data& pd, Camera* c) override;
    void handle_input(unsigned char key) override;
protected:
    WhirlwindForceGenerator* whirlwind;
    ExplosionForceGenerator* explosion;
};