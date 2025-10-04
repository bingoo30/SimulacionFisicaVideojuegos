#pragma once
#include "Entity.h"

class Particle : public Entity
{
public:
    Particle(
        const physx::PxVec3& p, // posición
        const physx::PxVec4& c, // color
        const physx::PxVec3& v, // velocidad
        const physx::PxVec3& a, // aceleración
        double d,  // damping
        IntegrateMode t,     // tipo de integración
        float m,     // masa
        double lt //tiempo de vida
    );

    void integrate(double t) override;

protected:
    void int_Euler(double t);
    void int_Euler_Semiimplicit(double t);
    void int_Verlet(double t);
    void calcular_damping(double t);

    physx::PxVec3 posAnt;  // posición anterior
    physx::PxVec3 vel;     // velocidad
    physx::PxVec3 acc;     // aceleración

    double damping;
    uint8_t integrMode;
};
