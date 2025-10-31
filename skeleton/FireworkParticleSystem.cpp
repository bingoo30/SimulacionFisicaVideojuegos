#include "FireworkParticleSystem.h"
#include "NormalGenerator.h"
#include "Particle.h"
#include <cmath>

using namespace physx;

FireworkParticleSystem::FireworkParticleSystem(
    const Particle_Data& pd,
    const Particle_Deviation_Data& pdd,
    int n)
    : ParticleSystem(new NormalGenerator(), pd, pdd, n, PxGeometryType::eSPHERE),
    initial_pos(pd.pos)
{
    spawnInterval = pd.vida;
    rocket_deviation = PxVec3(0, 0, 0);
    spark_deviation = PxVec3(pdd.vel.x, pdd.vel.y, pdd.vel.z);
}

bool FireworkParticleSystem::check_out_of_limit(Particle* p) const
{
    const float limit = (float)deviation.valid_box;
    const PxVec3& pos = p->getPosition();
    if (!exploded && abs(pos.y) > limit)
        return true;
    if (abs(pos.x) > limit*5 || pos.y<= 0.0f || abs(pos.z) > limit*5)
        return true;

    return false;
}

void FireworkParticleSystem::on_particle_removed(Particle* p)
{
    if (!exploded)
    {
        exploded = true;
        cooldown = 0.0;

        // Guardar la posición de la explosión
        explosion_pos = p->getPosition();
        spawnSpark();
    }
}

void FireworkParticleSystem::spawnRocket()
{
    Particle_Data rocket_model = model;
    Particle_Deviation_Data rocketDev = deviation;

    rocketDev.vel = rocket_deviation;
    rocketDev.ori = PxVec3(0, 0, 0);
    rocketDev.r_color = false;
    rocketDev.r_cant = false;

    // Usar siempre la posición inicial del cohete
    rocket_model.pos = initial_pos;

    auto particles = gen->generate_particles(rocket_model, rocketDev, num, PxGeometryType::eSPHERE);

    for (auto p : particles)
    {
        RegisterRenderItem(p->getRenderItem());
        particles_list.push_back(p);
    }
}

void FireworkParticleSystem::spawnSpark()
{
    Particle_Data spark_model;
    spark_model.pos = explosion_pos;  // usar la posición de la explosión
    spark_model.masa = model.masa;
    spark_model.vida = model.vida * 0.7;
    spark_model.volumen = model.volumen * 0.8;
    spark_model.color_offset = 8;
    spark_model.color_tam = 4;

    auto particles = gen->generate_particles(spark_model, deviation, num * 5, PxGeometryType::eSPHERE);

    for (auto p : particles)
    {
        RegisterRenderItem(p->getRenderItem());
        particles_list.push_back(p);
    }
}

void FireworkParticleSystem::update(double dt)
{
    for (auto it = particles_list.begin(); it != particles_list.end(); )
    {
        Particle* p = *it;
        p->update(dt);

        if (p->isDead() || check_out_of_limit(p))
        {
            on_particle_removed(p);
            DeregisterRenderItem(p->getRenderItem());
            delete p;
            it = particles_list.erase(it);
        }
        else
        {
            ++it;
        }
    }

    if (!exploded && particles_list.empty())
    {
        spawnRocket();
    }

    if (exploded)
    {
        cooldown += dt;
        if (cooldown >= spawnInterval)
        {
            exploded = false;
            cooldown = 0.0;
            spawnRocket();  // respawn en la posición inicial
        }
    }
}
