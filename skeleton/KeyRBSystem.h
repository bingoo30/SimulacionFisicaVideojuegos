#pragma once
#include "RigidBodySystem.h"
class KeyRBSystem :
    public RigidBodySystem
{
public:
    KeyRBSystem(const Particle_Data& pd, const physx::PxVec3& md, const physx::PxFilterData& dt);
    void init() override;
    Particle* getKey();
    void reset();
    void setInitPos(const physx::PxVec3& p);
protected:
    bool check_out_of_limit(Particle* p) const override;
    physx::PxVec3 initPos;
};

