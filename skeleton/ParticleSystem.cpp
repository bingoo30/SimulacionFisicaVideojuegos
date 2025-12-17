#include "ParticleSystem.h"
#include "SceneManager.h"
#include <algorithm>
using namespace std;
using namespace physx;
extern PxPhysics* gPhysics;

ParticleSystem::ParticleSystem(const Particle_Data& pd, const Particle_Deviation_Data& pdd, int n, physx::PxGeometryType::Enum geo, const physx::PxVec3& md) :generators(), particles_list(), model(pd), deviation(pdd), num(n), geometry(geo), local_registry(ForceRegistry()), force_generators(), mat(gPhysics->createMaterial(md.x, md.y, md.z))
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

void ParticleSystem::spawn(bool withRender, bool isStatic) 
{
    for (auto g : generators) {
        auto new_particles = g->generate_particles(model, deviation, num, geometry, mat, withRender);
        for (auto& new_p : new_particles) {
            // registrar todas las fuerzas locales sobre esta partícula
            if (!isStatic) {
                for (auto& fg : force_generators) {
                    local_registry.add_registry(new_p, fg.get());
                }
                local_registry.add_registry(new_p, SceneManager::instance().getCurrScene()->getGravityGenerator());
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
        if (p) p->update(dt);

    kill_dead_particles();

    //para los sistemas que no generan por el tiempo, lo controlo con este if
    if (spawn_acu < 0) return;
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
Particle_Data& ParticleSystem::getModel()
{
    return model;
}
void ParticleSystem::kill_dead_particles()
{
    particles_list.remove_if([this](std::unique_ptr<Particle>& p) {

        // si la partícula debe morir
        if (!p->is_alive() || check_out_of_limit(p.get())) {

            //eliminarla del ForceRegistry
            local_registry.clear_particle(p.get());

            //devolver true para que remove_if la borre de particles_list
            return true;
        }
        return false;
        });
}


