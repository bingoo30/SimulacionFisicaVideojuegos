#include "RigidBodySystem.h"
#include "Generator.h"
#include <algorithm>

using namespace physx;
extern PxPhysics* gPhysics;

RigidBodySystem::RigidBodySystem(const Particle_Data& pd,
    const Particle_Deviation_Data& pdd,
    int n,
    PxGeometryType::Enum geo,
    bool _static)
    : ParticleSystem(pd, pdd, n, geo), create_static(_static)
{
}

void RigidBodySystem::spawn()
{
    for (auto g : generators)
    {
        auto generated = g->generate_particles(model, deviation, num, geometry);

        for (Particle* p : generated)
        {
            // ------------------------------------------------------------------------------
            // Aquí sustituimos el Particle* generado por el generador por un RigidBody*
            // ------------------------------------------------------------------------------

            PxMaterial* mat = gPhysics->createMaterial(0.5f, 0.5f, 0.5f);

            PxShape* shape = nullptr;

            switch (geometry)
            {
            case PxGeometryType::eSPHERE:
                shape = gPhysics->createShape(PxSphereGeometry(model.vol), *mat);
                break;
            case PxGeometryType::eBOX:
                shape = gPhysics->createShape(PxBoxGeometry(model.vol, model.vol, model.vol), *mat);
                break;
            case PxGeometryType::eCAPSULE:
                shape = gPhysics->createShape(PxCapsuleGeometry(model.vol, model.vol), *mat);
                break;
            default:
                break;
            }

            RigidBody* rb = nullptr;

            if (create_static)
            {
                PxFilterData filter;
                rb = new StaticRigidBody(model, filter, shape, mat);
            }
            else
            {
                PxFilterData filter;
                rb = new DynamicRigidBody(model, mat, shape, PxVec3(-1), filter);
            }

            // registrar fuerzas locales
            for (auto& fg : force_generators)
                local_registry.add_registry(rb, fg.get());

            particles_list.push_back(std::unique_ptr<Particle>(rb));

            delete p;  // borrar la partícula generada por Generator (ya no se usa)
        }
    }
}


// ---------------------------------------------------------------------------
//   LIMITES DEL MUNDO
// ---------------------------------------------------------------------------
bool RigidBodySystem::check_out_of_limit(Particle* p) const
{
    RigidBody* rb = dynamic_cast<RigidBody*>(p);
    if (!rb) return true;

    PxVec3 pos = rb->getPosition();

    if (pos.y < -2000.0f) return true;
    if (abs(pos.x) > 10000.0f) return true;
    if (abs(pos.z) > 10000.0f) return true;

    return false;
}
