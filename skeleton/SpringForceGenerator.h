#pragma once
#include "ForceGenerator.h"
class Particle;
class SpringForceGenerator : public ForceGenerator
{
public:
    SpringForceGenerator(double k, double rl, Particle* p);

    virtual ~SpringForceGenerator() = default;
    void setK(double newK);
    void handle_special_input(int key) override;
    void handle_input(unsigned char key) override;
    void update_force(Particle* p, double dt) override;
protected:
    physx::PxVec3 calculate_force(Particle* p, double dt) override;
    double K; //coeficiente de elasticidad
    double resisting_length;
    Particle* other;
    const double umbral;

    bool extra_force_active = false;
    double extra_force_timer = 0.0;
    physx::PxVec3 extra_force_value = physx::PxVec3(100, 100, 0); 
};

