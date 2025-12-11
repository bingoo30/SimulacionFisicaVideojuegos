#pragma once
#include "Entity.h"

class Particle : public Entity
{
public:
    enum Type {
        DEFAULT, ROCKET, EXPLOSION, SPARK, NUM_TYPE
    };
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

    virtual ~Particle()=default;

    void integrate(double t) override;
    void update(double dt) override;

    virtual void add_force(const physx::PxVec3& f);
    void clean_force();

    void create_renderItem() override;

    bool is_type_of(Type t) const;
    void setType(Type t);

    void setDamping(double d) { damping = d; }

#pragma region getters
    virtual const physx::PxVec3& getPosition() const { return transform.p; };
    virtual const physx::PxVec3& getVelocity() const { return vel; };
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

    Type type = DEFAULT;

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
