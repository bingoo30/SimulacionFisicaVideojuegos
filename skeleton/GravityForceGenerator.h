#pragma once
#include "ForceGenerator.h"
#include "Particle.h"
class GravityForceGenerator : public ForceGenerator
{
public:
#pragma region constructora y destructora
    GravityForceGenerator(const physx::PxVec3& g);
    virtual ~GravityForceGenerator() = default;
#pragma endregion
#pragma region metodos publicos
    void update_force(Particle* p) override;
#pragma endregion
protected:
#pragma region atributos
    physx::PxVec3 gravity;
#pragma endregion
#pragma region metodos auxiliares
    physx::PxVec3 calculate_force(Particle* p) override;
#pragma endregion
};