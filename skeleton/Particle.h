#pragma once
#include "Entity.h"

class Particle : public Entity
{
public:
    Particle(
        const physx::PxVec3& p, // posición
        const physx::PxVec4& c, // color
        float m,     // masa
        physx::PxShape* sh, //tipo de geometria que uso,
        double vol,
        double lt, //tiempo de vida
        const physx::PxVec3& v, // velocidad
        IntegrateMode md,     // tipo de integración,
        double de = 1.225, //densidad del material (este es el de aire)
        double d = 0.95 // damping
    );

    ~Particle()=default;

    void integrate(double t) override;
    void update(double dt) override;

    void add_force(const physx::PxVec3& f);
    void clean_force();

#pragma region getters
    const physx::PxVec3& getPosition() const { return transform.p; };
    const physx::PxVec3& getVelocity() const { return vel; };
    double getRadius() const { return realRadius; };
#pragma endregion

protected:
    //atributos que inicializo con valor externo
    physx::PxVec3 last_pos;  // posición anterior
    physx::PxVec3 vel;     // velocidad
    double damping; //damping

    //atributos que inicializo con valor predefinido
    physx::PxVec3 acc = physx::PxVec3(0.0);     // aceleración
    physx::PxVec3 force= physx::PxVec3(0.0);;     // acumulador de fuerza

    double realRadius;
    const double density;
    virtual void update_force();
    void integrate_by_type(double dt);
    void calculate_real_radius();
private:
    void int_Euler(double t);
    void int_Euler_Semiimplicit(double t);
    void int_Verlet(double t);
    void calcular_damping(double t);
};
