#pragma once
#include "RigidBodySystem.h"
class GroundSystem :
    public RigidBodySystem 
{
public:
    GroundSystem(const Ground_Data& pd, int n, const physx::PxFilterData& dt = physx::PxFilterData(0,0,0,0), const physx::PxVec3& mt= physx::PxVec3(0.0));
    void init() override;
    void setMaterial(const physx::PxVec3& md);
    void spawn(bool _render = true, bool _isStatic = false) override;
protected:
    bool check_out_of_limit(Particle* p) const override;
};

