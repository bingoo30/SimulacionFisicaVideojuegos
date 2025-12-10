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
        bool _static);
    virtual ~RigidBodySystem() = default;

    //los futuros sistemas que heredan de esta clase tienen que hacer el override
    //virtual void init() override;
protected:
    // para crear cuerpos rígidos en lugar de partículas
    virtual void spawn() override;

    // para borrar cuerpos que salgan del área
    virtual bool check_out_of_limit(Particle* p) const override;
private:
    bool create_static;
};

