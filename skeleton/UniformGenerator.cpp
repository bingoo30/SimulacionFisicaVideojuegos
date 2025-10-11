#include "UniformGenerator.h"
#include "StructForEntities.h"
#include "Entity.h"
using namespace std;
using namespace physx;
std::list<Particle*> UniformGenerator::generate_particles() const
{
    //generadores
    std::mt19937 gen = _mt;
    std::uniform_real_distribution<double> u(-1.0, 1.0);
    std::list<Particle*> particles;
    for (int i = 0; i < n_particles; ++i) {
        //posicion aleatoria alrededor del origen
        physx::PxVec3 pos = ori + physx::PxVec3(u(gen),u(gen),u(gen));

        //velocidad aleatoria alrededor de la velocidad media
        PxVec3 vel_rand = vel + PxVec3(u(gen),u(gen),u(gen));

        // Crear partícula con posición, velocidad y duración base
        Particle* p = new Particle(pos, PxVec4(1.0f), vel_rand, CONST_GRAVITY, duration, Entity::VERLET, 0.05, );

        particles.push_back(p);
    }

    return particles;
}
