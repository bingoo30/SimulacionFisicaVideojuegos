#include "RigidBodySystem.h"
#include "Generator.h"
#include <algorithm>

using namespace physx;
extern PxPhysics* gPhysics;

RigidBodySystem::RigidBodySystem(const Particle_Data& pd,
    const Particle_Deviation_Data& pdd,
    int n,
    PxGeometryType::Enum geo,
    bool _static,
    const physx::PxVec3& md)
    : ParticleSystem(pd, pdd, n, geo, md), create_static(_static)
{
}

void RigidBodySystem::pause_all_rb(bool pause)
{
    for (auto& p : particles_list)
    {
        // Intentar convertir a rigid body dinámico
        if (auto* rb = dynamic_cast<DynamicRigidBody*>(p.get()))
        {
            auto dyn = static_cast<physx::PxRigidDynamic*>(rb->getActor());
            if (pause)
                dyn->putToSleep();
            else
                dyn->wakeUp();
        }
    }

}
