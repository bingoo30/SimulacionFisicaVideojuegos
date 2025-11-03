#include "Particle.h"
#include "StructForEntities.h"
using namespace physx;

Particle::Particle(const physx::PxVec3& p, const physx::PxVec4& c, float m, physx::PxShape* sh, double vol, double lt, const physx::PxVec3& v, IntegrateMode md, double d)
:Entity(p,c,m,sh,vol,lt,md), last_pos(p), vel(v), damping(d)
{
}

Particle::Particle(const Particle& other, bool create) :
	Entity(other.transform.p, other.color, other.mass, other.shape, other.volume, other.lifetime, other.mode),
	last_pos(other.transform.p), vel(other.vel), damping(other.damping), acc(other.acc), force(PxVec3(0.0))
{
}

Particle& Particle::operator=(const Particle& other)
{
	if (this != &other) {
		transform.p = other.transform.p;
		last_pos = other.transform.p;
		color = other.color;
		mass = other.mass;
		lifetime = other.lifetime;
		volume = other.volume;
		mode = other.mode;
		vel = other.vel;
		acc = other.acc;
		damping = other.damping;
		age = 0; 
		force = PxVec3(0.0);
	}
	return *this;
}

void Particle::integrate(double t)
{
	if (t <= 0.0) return; // evitar dt negativo

	switch (mode) {
	case IntegrateMode::EULER:
		int_Euler(t);
		break;
	case IntegrateMode::SEMI_IMPLICIT_EULER:
		int_Euler_Semiimplicit(t);
		break;
	case IntegrateMode::VERLET:
		int_Verlet(t);
		break;
	default:
		break;
	}
}

void Particle::update(double dt)
{
	update_lifetime(dt);
	if (!check_death()) return;
	update_force();
	integrate_by_type(dt);
	clean_force();
}

void Particle::add_force(const physx::PxVec3& f)
{
	force += f;
}

void Particle::clean_force()
{
	force = PxVec3(0.0);
}

void Particle::update_force()
{
	acc = CONST_GRAVITY + (force * (1.0/ mass));
}

void Particle::integrate_by_type(double dt)
{
	switch (mode)
	{
	case EULER:
		int_Euler(dt);
		break;
	case SEMI_IMPLICIT_EULER:
		int_Euler_Semiimplicit(dt);
		break;
	case VERLET:
		int_Verlet(dt);
		break;
	default:
		break;
	}
}

void Particle::int_Euler(double t)
{
	transform.p += t * vel;
	vel += t * acc;
	 
	calcular_damping(t);
}

void Particle::int_Euler_Semiimplicit(double t)
{
	vel += acc * t;
	//damping
	calcular_damping(t);
	transform.p += t * vel;
}

void Particle::int_Verlet(double t)
{
	// si es la primera integración, usamos Euler
	if (last_pos == transform.p) {
		int_Euler(t);
	}
	else {
		// guardamos posición actual
		PxVec3 posAct = transform.p;

		transform.p = 2.0f * transform.p - last_pos + acc * (t*t);
		vel = (transform.p - last_pos) / (2.0f * float(t));

		calcular_damping(t);

		// actualizamos posición anterior
		last_pos = posAct;
	}
}

void Particle::calcular_damping(double t)
{
	vel *= pow(damping, t);
}


