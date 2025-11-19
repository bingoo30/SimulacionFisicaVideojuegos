#pragma once
#include "ForceGenerator.h"
class Particle;
class SpringForceGenerator : public ForceGenerator
{
public:
    SpringForceGenerator(double k, double rl, Particle* p);

    virtual ~SpringForceGenerator() = default;
    void setK(double newK);
    void update_force(Particle* p, double dt) override;
    void add_force_by_key();
protected:
    physx::PxVec3 calculate_force(Particle* p, double dt) override;
    double K; //coeficiente de elasticidad
    double resisting_length;
    Particle* other;
};

