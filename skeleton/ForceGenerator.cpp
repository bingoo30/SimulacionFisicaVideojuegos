#include "ForceGenerator.h"
#include "Particle.h"
using namespace physx;

ForceGenerator::ForceGenerator():active(true)
{
}

void ForceGenerator::update_force(Particle* particle, double dt)
{
	//si no esta activo, dejo de actualizar las fuerzas
	if (!alive) return;
	//por si acaso verificar que existe esa particula
	if (particle != nullptr) {
		//evitar masa infinita
		if (particle->getMass() <= 0.0f) return;
		particle->add_force(calculate_force(particle,dt));
	}

}

void ForceGenerator::handle_special_input(int key)
{
}

void ForceGenerator::handle_input(unsigned char key)
{
}

bool ForceGenerator::is_alive() const
{
	return alive;
}

void ForceGenerator::set_alive(bool a)
{
	alive = a;
}
