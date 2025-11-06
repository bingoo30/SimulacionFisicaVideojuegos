#pragma once
#include "ForceGenerator.h"
class ExplosionForceGenerator :
    public ForceGenerator 
{
public:
    ExplosionForceGenerator(const physx::PxVec3& pos, double r, double K, double T);
    void update_force(Particle* p, double dt) override;
    virtual ~ExplosionForceGenerator() = default;

    void activate(bool a);
protected:
#pragma region atributos
    physx::PxVec3 center;   // Centro de la fuerza
    double radius;          // Radio
    double k;               // Intensidad
    double t;               // Constante de decaimiento temporal
    double elapsedTime;       // Tiempo transcurrido desde que comienza la explosión
    bool active;            // Si la explosión sigue activa
#pragma endregion

#pragma region metodos auxiliares
    physx::PxVec3 calculate_force(Particle* p, double dt) override;
#pragma endregion
};

