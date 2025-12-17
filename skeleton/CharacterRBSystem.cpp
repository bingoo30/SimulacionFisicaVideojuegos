#include "CharacterRBSystem.h"
#include "UniformGeneratorRB.h"
#include "SceneManager.h"
using namespace physx;
CharacterRBSystem::CharacterRBSystem(const Player_Data& pd, const physx::PxVec3& md, const physx::PxFilterData& dt): RigidBodySystem(pd, Particle_Deviation_Data(), 1, PxGeometryType::eBOX, false, md, dt)
{
}

void CharacterRBSystem::init()
{
	spawn_acu = -1;
    auto g = new UniformGeneratorRB(false);
    g->setFilter(filter);
	add_generator(g);
}

void CharacterRBSystem::spawn(bool withRender, bool isStatic)
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
            auto p = static_cast<DynamicRigidBody*>(new_p);
            PxRigidDynamic* dyn = static_cast<PxRigidDynamic*>(p->getActor());
            dyn->setAngularDamping(0.05f);
            dyn->setRigidDynamicLockFlags(
                PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
                PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z
            );
            /*

            PxQuat q(PxPi / 2.0f, PxVec3(0, 0, 1));
            p->add_initial_rotation(q);*/
            particles_list.push_back(std::unique_ptr<Particle>(new_p));
        }
        new_particles.clear();
    }

}

bool CharacterRBSystem::check_out_of_limit(Particle* p) const
{
	auto rb = static_cast<DynamicRigidBody*>(p);

	return rb->getPosition().y < -5;
}
