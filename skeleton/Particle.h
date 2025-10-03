#pragma once
#include "Entity.h"

class Particle : public Entity
{
public:
    enum IntegrateMode {
        EULER,
        EULER_SEMIIMPLICIT,
        VERLET
    };

    Particle(
        const physx::PxVec3& p = physx::PxVec3(0), // posici�n
        const physx::PxVec4& color = physx::PxVec4(1), // color
        const physx::PxVec3& v = physx::PxVec3(0), // velocidad
        const physx::PxVec3& a = physx::PxVec3(0), // aceleraci�n
        double d = 0.999,  // damping
        IntegrateMode t = IntegrateMode::EULER,     // tipo de integraci�n
        float m = 1.0f     // masa
    );

    void integrate(double t);

protected:
    void int_Euler(double t);
    void int_Euler_Semiimplicit(double t);
    void int_Verlet(double t);
    void calculate_damping(double t);

    physx::PxVec3 posAnt;  // posici�n anterior
    physx::PxVec3 vel;     // velocidad
    physx::PxVec3 acc;     // aceleraci�n

    double damping;
    uint8_t integrMode;
};
