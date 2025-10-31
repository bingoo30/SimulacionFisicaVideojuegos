#pragma once
#include "ParticleSystem.h"

class FireworkParticleSystem : public ParticleSystem {
public:
    FireworkParticleSystem(
        const Particle_Data& pd,
        const Particle_Deviation_Data& pdd,
        int n
    );

    void update(double dt) override;

protected:
    bool check_out_of_limit(Particle* p) const override;
    void on_particle_removed(Particle* p) override;

    void spawnRocket();
    void spawnSpark();

private:
    bool exploded = false;
    double cooldown = 0.0;
    double spawnInterval = 0.0;

    physx::PxVec3 rocket_deviation;
    physx::PxVec3 spark_deviation;

    physx::PxVec3 initial_pos;    // posición inicial del cohete
    physx::PxVec3 explosion_pos;  // posición donde explota el cohete
};

