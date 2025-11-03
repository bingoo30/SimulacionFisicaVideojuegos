#include "ParticleSystem.h"
#include <algorithm>
using namespace std;
ParticleSystem::ParticleSystem(Generator* g, const Particle_Data& pd, const Particle_Deviation_Data& pdd, int n, physx::PxGeometryType::Enum geo) :gen(g), particles_list(), model(pd), deviation(pdd), num(n), spawn_acu(0), spawn_period(0), geometry(geo)
{
}

ParticleSystem::~ParticleSystem()
{
	particles_list.clear();

	//liberar el generador
	delete gen;
	gen = nullptr;
}

void ParticleSystem::spawn()
{
	auto l = gen->generate_particles(model, deviation, num, geometry);
	particles_list.splice(particles_list.end(), l);
}

void ParticleSystem::update(double dt) {
    update_particles(dt);
    check_spawn(dt);
}

void ParticleSystem::derregister() {
	for (auto& p: particles_list) p->derregister_renderItem();
}

void ParticleSystem::register_particles()
{
    for (auto& p : particles_list) p->create_renderItem();
}

void ParticleSystem::add_particle(Particle* p)
{
    particles_list.push_back(unique_ptr<Particle>(p));
}

void ParticleSystem::update_particles(double dt)
{
    for (auto& it : particles_list)
    {
        it->update(dt);

        if (it->is_dead() || check_out_of_limit(it.get()))
        {
            it->kill();
        }
    }
    for (auto& it : particles_list) {
        if (!it->is_alive()) it->derregister_renderItem();
    }
}

void ParticleSystem::check_spawn(double dt)
{
    spawn_acu += dt;
    if (spawn_acu >= spawn_period) {
        int count = (int)(spawn_acu / spawn_period);
        for (int i = 0; i < count; ++i) spawn();
        spawn_acu = 0.0;
    }
}


