#include "ExplosionForceGenerator.h"
#include "Particle.h"
#include "StructForEntities.h"
#include <algorithm>
using namespace physx;
using namespace std;
ExplosionForceGenerator::ExplosionForceGenerator(const physx::PxVec3& pos, double r, double K, double T) :
    center(pos), radius(r), k(K), t(T), elapsedTime(0.0), active(false)
{
}

void ExplosionForceGenerator::update_force(Particle* p, double dt)
{
    if (!p) return;

    // Si no hay explosion activa, no aplicar fuerza
    if (!active) return;

    elapsedTime += dt;
    if (elapsedTime > 4.0 * t) {
        active = false; // la explosion ha terminado
        return;
    }

    // Calcular y aplicar la fuerza
    PxVec3 f = calculate_force(p, dt);
    p->add_force(f);
}

void ExplosionForceGenerator::activate(bool a)
{
    active = a;
    if (a) {
        elapsedTime = 0;
    }
}

PxVec3 ExplosionForceGenerator::calculate_force(Particle* p, double dt)
{
    PxVec3 pos = p->getPosition();
    PxVec3 diff = pos - center;
    double r = diff.magnitude();

    // aplicar solo dentro del radio fijo
    if (r < radius)
    {
        const double decay = exp(-elapsedTime / t);      // e^{-t/τ}
        const double fMagnitude = (k / (r * r)) * decay; // K/r² * decaimiento
        PxVec3 fDir = diff.getNormalized();
        return static_cast<float>(fMagnitude) * fDir;
    } else //fuera del radio
        return PxVec3(0.0f);
}
