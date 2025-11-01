#pragma once
#include "Entity.h"

class Particle : public Entity
{
public:
    Particle(
        const physx::PxVec3& p, // posición
        const physx::PxVec4& c, // color
        const physx::PxVec3& v, // velocidad
        IntegrateMode t,     // tipo de integración
        float m,     // masa
        double lt, //tiempo de vida
        physx::PxShape* sh, //tipo de geometria que uso
        double d = 0.995 // damping
    );
    virtual ~Particle();
    void integrate(double t) override;
    const physx::PxVec3& getPosition() const { return transform->p; };
    const physx::PxVec3& getVel() const { return vel; };

    void add_force(const physx::PxVec3& f);
    void clean_force();
protected:
    void int_Euler(double t);
    void int_Euler_Semiimplicit(double t);
    void int_Verlet(double t);
    void calcular_damping(double t);

    physx::PxVec3 pos_ant;  // posición anterior
    physx::PxVec3 vel;     // velocidad
    physx::PxVec3 acc;     // aceleración

    double damping;
    uint8_t integr_mode;
    physx::PxVec3 force;     // aceleración
};
