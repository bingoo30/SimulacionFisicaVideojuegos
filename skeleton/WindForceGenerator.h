#pragma once
#include "ForceGenerator.h"
class WindForceGenerator :
    public ForceGenerator 
{
public:
    WindForceGenerator(const physx::PxVec3& pos, const physx::PxVec3& v, const physx::PxVec3& wa, double _k1, double d);
    virtual ~WindForceGenerator() = default;
    void update_force(Particle* p) override;
#pragma region getters y setters
    void setK1(int k) { k1 = k; };
    int getK1() const { return k1; };
#pragma endregion
protected:
#pragma region atributos
    //velocidad del viento
    physx::PxVec3 vel;
    //area del viento
    physx::PxVec3 area;
    //centro
    physx::PxVec3 center;

    //coeficientes de rozamiento
    double k1; //c. rozamiento vel
    double k2; //c. square vel

    double density;
    double dragCoef;
#pragma endregion

#pragma region metodos auxiliares
    physx::PxVec3 calculate_force(Particle* p) override;
#pragma endregion
};

