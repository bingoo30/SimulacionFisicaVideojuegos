#include "FireworkParticleSystem.h"
#include "FireworkGenerator.h"
#include "Particle.h"
#include <cmath>
using namespace physx;

FireworkParticleSystem::FireworkParticleSystem(const Particle_Data& pd, const Particle_Deviation_Data& pdd, int n)
    : ParticleSystem(new FireworkGenerator(), pd, pdd, n, physx::PxGeometryType::Enum::eSPHERE)
{
    spawnInterval = 0.0; // sin spawn automatico
}

bool FireworkParticleSystem::check_out_of_limit(Particle* p) const
{
    if (p == nullptr) return true;

    const float limit = (float)deviation.valid_box;
    const PxVec3& pos = p->getPosition();

    bool outside =
        pos.x < -limit || pos.x > limit ||
        pos.y < -limit || pos.y > limit ||
        pos.z < -limit || pos.z > limit;

    return outside;
}

void FireworkParticleSystem::update(double dt)
{
    update_particles(dt); // sin auto-spawn
}

void FireworkParticleSystem::launch_firework()
{
    spawn(); // genera las partículas iniciales del fuego artificial
}
