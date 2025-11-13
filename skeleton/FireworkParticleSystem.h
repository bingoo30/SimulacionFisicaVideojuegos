#pragma once
#include "ParticleSystem.h"

/*
* model es la particula modelo de rocket
* deviation es su desviacion
* 
* generators[0]: generador para rocket
* generators[1]: generador para explosion
* generators[2]: generador para chispa
*/
class FireworkParticleSystem : public ParticleSystem {
public:
    FireworkParticleSystem(const Particle_Data& pd, const Particle_Deviation_Data& pdd);
    virtual ~FireworkParticleSystem();
    void init() override;

    void update(double dt) override;

    void launch_firework(); // llamado desde input

    void set_spark_data_and_deviation(const Particle_Data& d, const Particle_Deviation_Data& dv);
    void set_explosion_data_and_deviation(const Particle_Data& d, const Particle_Deviation_Data& dv);
protected:
    void create_explosion();
    void create_spark();
    bool check_out_of_limit(Particle* p) const override;
    Generator* spark_generator;
    Particle_Data spark_data;
    Particle_Deviation_Data spark_deviation;
    int n_sparks;

    Generator* explosion_generator;
    Particle_Data explosion_data;
    Particle_Deviation_Data explosion_deviation;
    int n_explosion;
};


