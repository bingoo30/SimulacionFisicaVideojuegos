#pragma once
#include "ParticleSystem.h"

class FireworkParticleSystem : public ParticleSystem {
public:
    FireworkParticleSystem(const Particle_Data& pd);
    virtual ~FireworkParticleSystem();
    void update(double dt) override;

    void launch_firework(); // llamado desde input
protected:
    void set_up();
    void create_explosion_at(const physx::PxVec3& pos, const physx::PxVec4& color);

    Generator* spark_generator;
    Particle_Data spark_data;
    Particle_Deviation_Data spark_deviation;

    Generator* explosion_generator;
    Particle_Data explosion_data;
    Particle_Deviation_Data explosion_deviation;
    int n_explosion;

    physx::PxVec4 rocket_color;
    physx::PxVec4 explosion_color;
    int n_sparks;
};


