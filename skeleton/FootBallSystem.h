#pragma once
#include "RigidBodySystem.h"
class FootBallSystem :
    public RigidBodySystem
{
public:
    FootBallSystem(const Football_Data& pd, const Football_Deviation_Data& pdd, int n, const physx::PxVec3& md);
    void init() override;
protected:
    bool check_out_of_limit(Particle* p) const override;
};
