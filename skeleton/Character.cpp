#include "Character.h"
#include <PxPhysicsAPI.h>
#include <cmath>
#include <algorithm>
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
    // Inicializa dirección y parámetros
    direction = physx::PxVec3(1, 0, 1).getNormalized();
    angle = 0.0f;
    rotation_speed = 5.0f;
}


void Character::update(double dt)
{
    // Si esta cargando fuerza, incrementa
    if (charging_force)
    {
        current_force = std::min<float>(current_force + charge_rate * (float)dt, max_force);
    }

    // Aplica física normal de la partícula
    // No compruebo tiempo de vida porque no la tiene
    update_force();
    integrate_by_type(dt);
    clean_force();

    //Evitar que caiga por debajo del suelo
    if (transform.p.y < 0.0f)
    {
        // Corrige la posición
        transform.p.y = 0.0f;

        // Anula la velocidad vertical
        if (vel.y < 0.0f)
            vel.y = 0.0f;
    }
}

void Character::update_direction(bool right)
{
    angle += (right ? rotation_speed : -rotation_speed);

    // rotación en el plano XZ
    float radians = physx::PxPi * angle / 180.0f;
    physx::PxVec3 baseDir = physx::PxVec3(1, 0, 1).getNormalized();

    // Rota el vector base alrededor del eje Y
    float cosA = cos(radians);
    float sinA = sin(radians);

    direction = physx::PxVec3(
        baseDir.x * cosA - baseDir.z * sinA,
        0,
        baseDir.x * sinA + baseDir.z * cosA
    ).getNormalized();
}


