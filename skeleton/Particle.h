#pragma once
#include "Entity.h"
#include <functional>
class ParticleSystem;
enum IntegrateMode {
    EULER,
    EULER_SEMIIMPLICIT,
    VERLET
};
enum ParticleType {
    NORMAL,
    FIREWORK_ROCKET,
    FIREWORK_SPARK
};
class Particle : public Entity
{
using Callback = std::function<void(ParticleSystem*, const Particle*)>;
public:
#pragma region constructoras y desturctora
    //constructora por valor
    Particle(
        const physx::PxVec3& p, // posición
        const physx::PxVec4& c, // color
        double m, // masa
        physx::PxShape* sh, //tipo de geometria que uso
        double vol,//volumen
        double lt, //tiempo de vida
        const physx::PxVec3& v, // velocidad
        IntegrateMode t,     // tipo de integración
        ParticleType pt = NORMAL, //tipo de particula (diferenciado para el fuego artificial)
        double d = 0.995 // damping
    );
    //constructora por copia
    Particle(const Particle& other, bool create);
    //operador asignacion
    Particle& operator=(const Particle& other);

    virtual ~Particle() = default;
#pragma endregion
#pragma region metodos para el update
    void update(double dt) override;
    void integrate(double dt) override;
    void update_force();
#pragma endregion
#pragma region metodos relacionados con la fuerza
    void add_force(const physx::PxVec3& f) { force += f; };
    void clean_force() { force = physx::PxVec3(0.0); };
#pragma endregion
#pragma region eventos
    void set_on_death(Callback cb) { on_death = cb; };
    void trigger_death(ParticleSystem* sys) const { if (on_death && sys != nullptr) on_death(sys, this); };
#pragma endregion
#pragma region getters
    const physx::PxVec3& getPosition() const { return transform.p; };
    const physx::PxVec3& getVel() const { return vel; };
    const physx::PxVec3& getAcc() const { return acc; };
    IntegrateMode getMode() const { return integr_mode; };
    ParticleType getType() const { return par_type; };
#pragma endregion
#pragma region setters
    void setParticleType(ParticleType t) { par_type = t; };
#pragma endregion
protected:
#pragma region atributos
    physx::PxVec3 vel;     // velocidad
    physx::PxVec3 acc;     // aceleración

    double damping;
    IntegrateMode integr_mode;
    ParticleType par_type;
    physx::PxVec3 force;     // aceleración
    physx::PxVec3 last_pos;  // posición anterior
    Callback on_death;
#pragma endregion
#pragma region metodos de integracion
    void int_Euler(double t);
    void int_Euler_Semiimplicit(double t);
    void int_Verlet(double t);
    void calcular_damping(double t);
#pragma endregion
};
