#include "ParticleSystem.h"
#include <algorithm>
using namespace std;
ParticleSystem::ParticleSystem(Generator* g, const Particle_Data& pd, const Particle_Deviation_Data& pdd, int n, physx::PxGeometryType::Enum geo) :gen(g), particles_list(), model(pd), deviation(pdd), num(n), spawnAccumulator(0), spawnInterval(0), geometry(geo)
{
}

ParticleSystem::~ParticleSystem()
{
	//liberar la lista
	for (auto p : particles_list) {
		DeregisterRenderItem(p->getRenderItem());
		delete p;
	}
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
    for (auto it = particles_list.begin(); it != particles_list.end();) {
        Particle* p = *it;
        p->update(dt);

        if (p->isDead() || check_out_of_limit(p)) {
            DeregisterRenderItem(p->getRenderItem());
            it = particles_list.erase(it);
        }
        else {
            ++it;
        }
    }

    spawnAccumulator += dt;
    if (spawnAccumulator >= spawnInterval) {
        int spawnCount = (int)(spawnAccumulator / spawnInterval);
        for (int i = 0; i < spawnCount; ++i) spawn();
        spawnAccumulator = 0.0;
    }
}

void ParticleSystem::derregister() {
	for (auto p: particles_list) DeregisterRenderItem(p->getRenderItem());
}

void ParticleSystem::register_particles()
{
    for (auto p : particles_list) RegisterRenderItem(p->getRenderItem());
}


