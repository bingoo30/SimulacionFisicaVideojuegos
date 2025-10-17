#include "ParticleSystem.h"
ParticleSystem::ParticleSystem(Generator* g, const Particle_Data& pd, const Particle_Deviation_Data& pdd, int n) :gen(g), particles_list(), model(pd), deviation(pdd), num(n)
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
	auto& l = gen->generate_particles(model, deviation, num);
	particles_list.splice(particles_list.end(), l);
}

void ParticleSystem::update(double dt)
{
	for (auto p : particles_list) {
		p->integrate(dt);
	}
}

void ParticleSystem::erase()
{
	for (auto p : particles_list) {
		if (p->isDead()) DeregisterRenderItem(p->getRenderItem());
	}
}

