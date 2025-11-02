#include "FireworkParticleSystem.h"
#include "NormalGenerator.h"
#include "Particle.h"
#include <cmath>

using namespace physx;

FireworkParticleSystem::FireworkParticleSystem(const Particle_Data& pd, const Particle_Deviation_Data& pdd, int n) : ParticleSystem(new NormalGenerator(), pd, pdd, n, physx::PxGeometryType::Enum::eSPHERE)
{
    spawnInterval = 0.05;
}

bool FireworkParticleSystem::check_out_of_limit(Particle* p) const
{
    // Cubo de interés centrado en el origen
    const float limit = static_cast<float>(deviation.valid_box);
    const PxVec3& pos = p->getPosition();

    // Solo aplicar límites si es una chispa (fase de explosión)
    if (p)
    {
        bool outside =
            pos.x < -limit || pos.x > limit ||
            pos.y < -limit || pos.y > limit ||
            pos.z < -limit || pos.z > limit;

        return outside;
    }
}
