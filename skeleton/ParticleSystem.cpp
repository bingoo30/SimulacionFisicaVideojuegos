#include "ParticleSystem.h"
#include <algorithm>
using namespace std;
ParticleSystem::ParticleSystem(const Particle_Data& pd, const Particle_Deviation_Data& pdd, int n, physx::PxGeometryType::Enum geo) :generators(), particles_list(), model(pd), deviation(pdd), num(n), geometry(geo), local_registry(ForceRegistry()), force_generators()
{
}

ParticleSystem::~ParticleSystem()
{
    particles_list.clear();

    for (auto g : generators) {
        delete g;
    }
    generators.clear();
}

void ParticleSystem::spawn()
{
    for (auto g : generators) {
        auto new_particles = g->generate_particles(model, deviation, num, geometry);
        for (auto& new_p : new_particles) {
            // registrar todas las fuerzas locales sobre esta partícula
            for (auto& fg : force_generators) {
                local_registry.add_registry(new_p, fg.get());
            }

            particles_list.push_back(std::unique_ptr<Particle>(new_p));
        }
        new_particles.clear();
    }
}

void ParticleSystem::update(double dt) {
    if (!active) return;
    // actualiza las fuerzas de este sistema
    local_registry.update_forces(dt);

    // actualiza el movimiento de las partículas
    for (auto& p : particles_list)
        p->update(dt);

    kill_dead_particles();

    // re-generar partículas si toca
    spawn_acu += dt;
    if (spawn_acu >= spawn_period) {
        spawn();
        spawn_acu = 0.0;
    }
}

void ParticleSystem::derregister()
{
    for (auto& particle : particles_list) {
        if (particle) particle->derregister_renderItem();
    }
}
void ParticleSystem::register_particles() 
{
    for (auto& particle : particles_list) {
        if (particle && !particle->is_valid_renderItem()) {
            particle->create_renderItem();
        }
    }
}

void ParticleSystem::init()
{
}

void ParticleSystem::add_generator(Generator* gen)
{
    if (gen != nullptr);
    generators.push_back(gen);
}
void ParticleSystem::add_force_generator(ForceGenerator* gen)
{
    if (gen != nullptr) {
        force_generators.push_back(std::unique_ptr<ForceGenerator>(gen));
        for (auto& p : particles_list) {
            local_registry.add_registry(p.get(), gen);
        }
    }
}
bool ParticleSystem::is_active() const
{
    return active;
}
void ParticleSystem::set_Active(bool a)
{
    active = a;
}
void ParticleSystem::kill_dead_particles()
{
    particles_list.remove_if([this](std::unique_ptr<Particle>& p) {
        return  !p->is_alive() || check_out_of_limit(p.get());
        });
}

