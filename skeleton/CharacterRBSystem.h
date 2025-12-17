#pragma once
#include "RigidBodySystem.h"
class CharacterRBSystem :
    public RigidBodySystem
{
public:
    CharacterRBSystem(const Player_Data& pd, const physx::PxVec3& md, const physx::PxFilterData& dt);
    virtual ~CharacterRBSystem() = default;
    void init() override;
    void spawn(bool withRender = true, bool isStatic = false) override;
protected:
    bool check_out_of_limit(Particle* p) const override;
};

