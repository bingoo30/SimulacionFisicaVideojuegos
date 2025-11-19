#pragma once
#include "ForceGenerator.h"
class WindForceGenerator :
    public ForceGenerator 
{
public:
    WindForceGenerator(const physx::PxVec3& pos, const physx::PxVec3& v, double r, double _k1, double d, bool moreRealistic);
    virtual ~WindForceGenerator() = default;
#pragma region getters y setters
    void setK1(int k) { k1 = k; };
    int getK1() const { return k1; };
#pragma endregion
protected:
#pragma region atributos
    //velocidad del viento
    physx::PxVec3 vel;
    //radio del area del viento
    double radius;
    //centro
    physx::PxVec3 center;

    //coeficientes de rozamiento
    double k1; //c. rozamiento vel
    double k2; //c. square vel

    double density;
    double dragCoef;

    bool realistic;
#pragma endregion

#pragma region metodos auxiliares
    physx::PxVec3 calculate_force(Particle* p, double dt = 0.0) override;
#pragma endregion
};

