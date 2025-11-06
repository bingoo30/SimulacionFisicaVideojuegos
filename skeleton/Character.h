#pragma once
#include "Particle.h"
#include "StructForEntities.h"
class Character : public Particle
{
public:
    Character(
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
    ~Character()=default;

    void update(double dt) override;
    void update_direction(bool right);

#pragma region getters y setters
    void setChargingForce(bool s) { charging_force = s; };
    void setCurrentForce(float f) { current_force = f; };

    const physx::PxVec3& getDirection() const { return direction; };
    float getCurrentForceMagnitude() const { return current_force; };
#pragma endregion

protected:
    bool charging_force = false;
    float current_force = 0.0f;
    float max_force = 500.0f;
    float charge_rate = 100.0f; // unidades por segundo
    float angle; // ángulo de rotación en grados
    physx::PxVec3 direction;  // empieza mirando hacia +X
    float rotation_speed;   // grados por pulsación de flecha

};
