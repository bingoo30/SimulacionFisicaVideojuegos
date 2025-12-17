#pragma once
#include "RigidBodySystem.h"
class FireRBSystem :
    public RigidBodySystem
{
public:
    FireRBSystem(const Fire_Particle_Data& pd, const Fire_Deviation_Data& pdd, int n,const physx::PxVec3& md, const physx::PxFilterData& dt);
    virtual ~FireRBSystem() = default;
    void init() override;
protected:
    bool check_out_of_limit(Particle* p) const override;
};

