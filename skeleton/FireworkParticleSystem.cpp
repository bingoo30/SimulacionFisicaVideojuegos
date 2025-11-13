#include "FireworkParticleSystem.h"
#include "Particle.h"
#include "UniformGenerator.h"
#include "NormalGenerator.h"
#include <cmath>
using namespace physx;

FireworkParticleSystem::FireworkParticleSystem(const Particle_Data& pd, const Particle_Deviation_Data& pdd)
    : ParticleSystem(pd, pdd, 1, PxGeometryType::Enum::eSPHERE)
{
}

FireworkParticleSystem::~FireworkParticleSystem()
{
    delete explosion_generator;
}


void FireworkParticleSystem::launch_firework()
{
    auto p = new Particle(
        model.pos, 
        model.color, 
        model.mass, 
        CreateShape(physx::PxSphereGeometry(model.vol)), 
        model.vol, 
        model.lifetime, 
        model.vel, 
        model.mode);
    p->setType(Particle::ROCKET);
    p->create_renderItem();
    particles_list.push_back(std::unique_ptr<Particle>(p));

}

void FireworkParticleSystem::set_spark_data_and_deviation(const Particle_Data& d, const Particle_Deviation_Data& dv)
{
    spark_data = d; spark_deviation = dv;
}

void FireworkParticleSystem::set_explosion_data_and_deviation(const Particle_Data& d, const Particle_Deviation_Data& dv)
{
    explosion_data = d; explosion_deviation = dv;
}

void FireworkParticleSystem::init()
{
    spawn_period = 0.0; // sin spawn automatico

    add_generator(new NormalGenerator()); //generador rocket
    add_generator(new UniformGenerator()); //generador explosion
    add_generator(new UniformGenerator()); //generador chispa
}

void FireworkParticleSystem::create_explosion()
{
    auto expParticles = generators[1]->generate_particles(explosion_data, explosion_deviation, n_explosion, PxGeometryType::Enum::eSPHERE);
    for (auto& explosionParticle : expParticles) {
        explosionParticle->setType(Particle::EXPLOSION);
        particles_list.push_back(std::unique_ptr<Particle>(explosionParticle));
        for (auto& force : force_generators) {
            local_registry.add_registry(explosionParticle, force.get());
        }
    }
}

void FireworkParticleSystem::create_spark()
{
    auto sparks = generators[2]->generate_particles(spark_data, spark_deviation, n_sparks, PxGeometryType::Enum::eSPHERE);
    for (auto& spark : sparks) {
        particles_list.push_back(std::unique_ptr<Particle>(spark));
        for (auto& force : force_generators) {
            local_registry.add_registry(spark, force.get());
        }
    }
}

bool FireworkParticleSystem::check_out_of_limit(Particle* p) const
{
    return false;
}

void FireworkParticleSystem::update(double dt)
{

    local_registry.update_forces(dt);

    std::vector<Particle*> deadRockets;
    std::vector<Particle*> deadExplosion;

    for (auto& p : particles_list) {
        p->update(dt);

        if (!p->is_alive()) {
            if (p->is_type_of(Particle::ROCKET)) {
                deadRockets.push_back(p.get());
            }
            else if (p->is_type_of(Particle::EXPLOSION)) {
                deadRockets.push_back(p.get());
            }
        }
    }

    for (auto* rocket : deadRockets) {
        explosion_data.pos = rocket->getPosition();
        create_explosion();
    }

    for (auto* ex : deadExplosion) {
        spark_data.pos = ex->getPosition();
        create_spark();
    }

    particles_list.remove_if([this](std::unique_ptr<Particle>& p) {
        if (!p->is_alive()) {
            local_registry.clear_particle(p.get());
            return true;
        }
        return false;
    });
}
