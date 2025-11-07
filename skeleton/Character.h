#pragma once
#include "Particle.h"
#include "StructForEntities.h"
#include <PxPhysicsAPI.h>

class Character : public Particle
{
public:
    Character(
        const physx::PxVec3& p,      // posición inicial
        const physx::PxVec4& c,      // color
        float m,                     // masa
        physx::PxShape* sh,          // tipo de geometría
        double vol,
        double lt,                   // tiempo de vida
        const physx::PxVec3& v,      // velocidad inicial
        IntegrateMode md,            // tipo de integración
        double de = 1.225,           // densidad del material (aire por defecto)
        double d = 0.95              // damping
    );

    ~Character() = default;

    // --- Métodos principales ---
    void update(double dt) override;
    void update_force() override;
    void jump();

    // --- Control de movimiento lateral ---
    void move_left();
    void move_right();
    void stop_horizontal();

    // --- Control de carga de salto ---
    void setChargingForce(bool s) { charging_force = s; }
    void setCurrentForce(float f) { current_force = f; }

    // --- Getters ---
    const physx::PxVec3& getDirection() const { return direction; }
    float getCurrentForceMagnitude() const { return current_force; }

protected:
    // --- Movimiento y física ---
    bool charging_force = false;
    float current_force = 0.0f;
    float max_force = 3000.0f;
    float charge_rate = 1000.0f;   // velocidad de carga de salto
    float move_speed = 15.0f;      // velocidad lateral

    // --- Estado de orientación ---
    physx::PxVec3 direction;       // dirección hacia la que mira (+X o -X)
};
