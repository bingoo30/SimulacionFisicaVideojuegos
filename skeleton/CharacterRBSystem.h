#pragma once
#include "RigidBodySystem.h"
class CharacterRBSystem :
    public RigidBodySystem
{
public:
    CharacterRBSystem(const Player_Data& pd, const physx::PxVec3& md);
    virtual ~CharacterRBSystem() = default;
    void init() override;
    void spawn(bool withRender = true) override;
protected:
    bool check_out_of_limit(Particle* p) const override;
};

