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
    const physx::PxVec3& md, 
    const physx::PxFilterData& dt)
    : ParticleSystem(pd, pdd, n, geo, md), create_static(_static), filter(dt)
{
}

void RigidBodySystem::pause_all_rb(bool pause)
{
    for (auto& p : particles_list)
    {
        if (p) {
            if (auto* rb = dynamic_cast<DynamicRigidBody*>(p.get())) {
                physx::PxRigidActor* actor = rb->getActor();
                if (actor) {
                    physx::PxRigidDynamic* dyn = actor->is<physx::PxRigidDynamic>();
                    if (dyn) {
                        if (pause) dyn->putToSleep();
                        else dyn->wakeUp();
                    }
                }
            }
        }
    }
}

void RigidBodySystem::spawn(bool withRender, bool isStatic)
{
    for (auto g : generators) {
        auto new_particles = g->generate_particles(model, deviation, num, geometry, mat, withRender);
        for (auto& new_p : new_particles) {
            // registrar todas las fuerzas locales sobre esta partícula
            if (!isStatic) {
                for (auto& fg : force_generators) {
                    local_registry.add_registry(new_p, fg.get());
                }
            }
            particles_list.push_back(std::unique_ptr<Particle>(new_p));
        }
        new_particles.clear();
    }
}

