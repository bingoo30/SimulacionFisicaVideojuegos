#pragma once
#include "Entity.h"

class Particle : public Entity
{
public:
    Particle(
        const physx::PxVec3& p, // posici�n
        const physx::PxVec4& c, // color
        const physx::PxVec3& v, // velocidad
        const physx::PxVec3& a, // aceleraci�n
        double d,  // damping
        IntegrateMode t,     // tipo de integraci�n
        float m,     // masa
        double lt //tiempo de vida
    );

    void integrate(double t) override;

protected:
    void int_Euler(double t);
    void int_Euler_Semiimplicit(double t);
    void int_Verlet(double t);
    void calcular_damping(double t);

    physx::PxVec3 posAnt;  // posici�n anterior
    physx::PxVec3 vel;     // velocidad
    physx::PxVec3 acc;     // aceleraci�n

    double damping;
    uint8_t integrMode;
};
