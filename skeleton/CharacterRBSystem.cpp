#include "CharacterRBSystem.h"
#include "UniformGeneratorRB.h"
#include "SceneManager.h"
using namespace physx;
CharacterRBSystem::CharacterRBSystem(const Player_Data& pd, const physx::PxVec3& md): RigidBodySystem(pd, Particle_Deviation_Data(), 1, PxGeometryType::eCAPSULE, false, md)
{
}

void CharacterRBSystem::init()
{
	spawn_acu = -1;
	add_generator(new UniformGeneratorRB(false));
}

void CharacterRBSystem::spawn(bool withRender)
{
    for (auto g : generators) {
        auto new_particles = g->generate_particles(model, deviation, num, geometry, mat, withRender);
        for (auto& new_p : new_particles) {
            // registrar todas las fuerzas locales sobre esta partícula
            for (auto& fg : force_generators) {
                local_registry.add_registry(new_p, fg.get());
            }
            static_cast<DynamicRigidBody*>(new_p)->add_initial_rotation((PxQuat(cos(45), sin(45), 0, 0)));
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
