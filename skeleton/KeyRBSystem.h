#pragma once
#include "RigidBodySystem.h"
class KeyRBSystem :
    public RigidBodySystem
{
public:
    KeyRBSystem(const Particle_Data& pd, const physx::PxVec3& md, const physx::PxFilterData& dt);
    void init() override;
    Particle* getKey();
protected:
    bool check_out_of_limit(Particle* p) const override;
};

