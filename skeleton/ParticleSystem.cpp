#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(): num(0), gen(nullptr), particles_list()
{
}

ParticleSystem::ParticleSystem(int n, Generator* g): num(n), gen(g), particles_list()
{
	init();
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
	gen->generate_particles(pos_d, vel_d, mas_d, dur_d, r_color, r_cant);
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

