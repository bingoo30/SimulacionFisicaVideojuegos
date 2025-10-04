#pragma once
#include "Particle.h"
class Projectile :
    public Particle
{
public: 
    Projectile(const physx::PxVec3& p,
        const physx::PxVec4& c,
        const physx::PxVec3& v,
        const physx::PxVec3& a,
        double d,
        float m,
        double lt);
    virtual ~Projectile();
protected:
    void integrate(double t) override;
    float calcular_masa_simulada(float mr, float vr, float vs);
    float calcular_gravedad_simulada(float gr, float vr, float vs);
};

