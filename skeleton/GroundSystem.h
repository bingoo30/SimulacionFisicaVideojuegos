#pragma once
#include "RigidBodySystem.h"
class GroundSystem :
    public RigidBodySystem 
{
public:
    GroundSystem(const Ground_Data& pd, int n, const physx::PxVec3& md);
    void init() override;
protected:
    bool check_out_of_limit(Particle* p) const override;
};

