#include "Character.h"
#include <PxPhysicsAPI.h>
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace physx;

Character::Character(
    const PxVec3& p,
    const PxVec4& c,
    float m,
    PxShape* sh,
    double vol,
    double lt,
    const PxVec3& v,
    IntegrateMode md,
    double de,
    double d
) : Particle(p, c, m, sh, vol, lt, v, md, de, d)
{
    direction = PxVec3(1, 0, 0); // empieza mirando hacia +X
}

void Character::update(double dt)
{
    // Solo carga si está en el suelo Y si aún no ha saltado
    if (charging_force && transform.p.y < 0.05f)
    {
        current_force = std::min<float>(current_force + charge_rate * dt, max_force);
    }

    update_force();
    integrate_by_type(dt);
    clean_force();

    // Mantener sobre el suelo
    if (transform.p.y < 0.0f)
    {
        transform.p.y = 0.0f;
        if (vel.y < 0.0f) vel.y = 0.0f;
    }

    // Frenado lateral
    if (std::abs(vel.x) > 0.01f)
        vel.x *= 0.85f;
    else
        vel.x = 0.0f;
}

void Character::update_force()
{
    // Evitar empuje hacia abajo al tocar el suelo
    if (transform.p.y < 0.0f) {
        transform.p.y = 0.0f;
        if (vel.y < 0.0f) vel.y = 0.0f;
    }

    // Aceleración = fuerza / masa
    acc = force * (1.0f / mass);
}

// --- Movimiento lateral ---

void Character::move_left()
{
    direction = PxVec3(-1, 0, 0);
    add_force(direction * move_speed * mass); // fuerza lateral coherente con la masa
}

void Character::move_right()
{
    direction = PxVec3(1, 0, 0);
    add_force(direction * move_speed * mass);
}

void Character::stop_horizontal()
{
    vel.x = 0.0f; // detiene movimiento lateral al soltar tecla
}

// --- Salto ---

void Character::jump()
{
    // Solo salta si está en el suelo
    if (transform.p.y < 0.05f)
    {
        // Escala de fuerza acumulada
        float jumpPower = std::clamp(current_force, 0.0f, max_force);

        // Aplica fuerza vertical hacia arriba
        PxVec3 jumpDir(0.0f, 1.0f, 0.0f);
        add_force(jumpDir * jumpPower * mass); // fuerza = m * a

        // Reinicia carga
        current_force = 0.0f;
        charging_force = false;
    }
}
