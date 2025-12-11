#pragma once
#include "ParticleSystem.h"
#include "DynamicRigidBody.h"
#include "StaticRigidBody.h"

class RigidBodySystem :
    public ParticleSystem
{
public:
    RigidBodySystem(const Particle_Data& pd,
        const Particle_Deviation_Data& pdd,
        int n,
        physx::PxGeometryType::Enum geo,
        bool _static, 
        const physx::PxVec3& md);
    virtual ~RigidBodySystem() = default;

    void pause_all_rb(bool pause);

    //los futuros sistemas que heredan de esta clase tienen que hacer el override
    //virtual void init() override;
    //tambien el check out of limit
protected:
    bool create_static;


};

