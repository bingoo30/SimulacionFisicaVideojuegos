#include "FireworkParticleSystem.h"
#include "Particle.h"
#include "UniformGenerator.h"
#include <cmath>
using namespace physx;

FireworkParticleSystem::FireworkParticleSystem(const Particle_Data& pd)
    : ParticleSystem(pd, Particle_Deviation_Data(), 1, PxGeometryType::Enum::eSPHERE)
{
    spawn_period = 0.0; // sin spawn automatico
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
    p->create_renderItem();
    particles_list.push_back(std::unique_ptr<Particle>(p));

}

void FireworkParticleSystem::set_up()
{
    spark_generator = new UniformGenerator();
    explosion_generator = new UniformGenerator();
    add_generator(explosion_generator); //generador explosion
    add_generator(spark_generator); //generador chispa
}

void FireworkParticleSystem::create_explosion_at(const PxVec3& pos, const PxVec4& color)
{
    auto expParticles = explosion_generator->generate_particles(explosion_data, explosion_deviation, n_explosion, PxGeometryType::Enum::eSPHERE);
    for (auto& explosionParticle : expParticles) {
        particles_list.push_back(std::unique_ptr<Particle>(explosionParticle));
        for (auto& force : force_generators) {
            local_registry.add_registry(explosionParticle, force.get());
        }
    }
}

void FireworkParticleSystem::update(double dt)
{

    local_registry.update_forces(dt);

    std::vector<Particle*> deadRockets;
    std::vector<Particle*> deadExplosion;

    for (auto& p : particles_list) {
        p->update(dt);

        if (!p->is_alive()) {
            const PxVec4& color = p->getColor();

            if (color == rocket_color) {
                deadRockets.push_back(p.get());
            }
            else if (color == explosion_color) {
                deadRockets.push_back(p.get());
            }
        }
    }

    for (auto* rocket : deadRockets) {
        create_explosion_at(rocket->getPosition(), explosion_color);
    }

    for (auto* ex : deadExplosion) {
        spark_data.pos = ex->getPosition();
        auto sparks = spark_generator->generate_particles(spark_data, spark_deviation, n_sparks, PxGeometryType::Enum::eSPHERE);
        for (auto& spark : sparks) {
            particles_list.push_back(std::unique_ptr<Particle>(spark));
            for (auto& force : force_generators) {
                local_registry.add_registry(spark, force.get());
            }
        }
    }

    particles_list.remove_if([this](std::unique_ptr<Particle>& p) {
        if (!p->is_alive()) {
            local_registry.clear_particle(p.get());
            return true;
        }
        return false;
    });
}
