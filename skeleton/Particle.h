#pragma once
#include "Entity.h"
#include <functional>
class ParticleSystem;
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
        double vol,
        double d = 0.995 // damping
    );
    virtual ~Particle();
    void integrate(double t) override;
    
    //eventos
    void set_on_death(std::function<void(ParticleSystem&, const Particle&)> cb);
    void trigger_death(ParticleSystem* sys) const;


    const physx::PxVec3& getPosition() const { return transform->p; };
    const physx::PxVec3& getVel() const { return vel; };
    const IntegrateMode getType() const { return integr_mode; };

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
    IntegrateMode integr_mode;
    physx::PxVec3 force;     // aceleración

    std::function<void(ParticleSystem&, const Particle&)> on_death;
};
