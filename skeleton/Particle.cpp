#include "Particle.h"
#include "ParticleSystem.h"
using namespace physx;
#pragma region constructoras
Particle::Particle(const PxVec3& p, const PxVec4& c, double m, PxShape* sh, double vol, double lt, const PxVec3& v, IntegrateMode t, ParticleType pt, double d)
	:Entity(p, c, m, sh, vol, lt),
	vel(v), integr_mode(t), par_type(pt), damping(d), force(PxVec3(0.0)), acc(PxVec3(0.0)), last_pos(p)
{
}
Particle::Particle(const Particle& other, bool create)
	: Entity(other.transform.p, other.color, other.mass, other.shape, other.volume, other.lifetime, create),
	vel(other.vel),
	integr_mode(other.integr_mode),
	par_type(other.par_type),
	damping(other.damping),
	force(PxVec3(0.0)),
	acc(other.acc),
	last_pos(other.transform.p)
{
}
Particle& Particle::operator=(const Particle& other)
{
	if (this != &other) {
		transform.p = other.transform.p;
		vel = other.vel;
		acc = other.acc;
		mass = other.mass;
		damping = other.damping;
		lifetime = other.lifetime;
		integr_mode = other.integr_mode;
		age = 0; // Reiniciar tiempo
		last_pos = other.transform.p;
		color = other.color;
	}
	return *this;
}
#pragma endregion
#pragma region update
void Particle::update(double dt)
{
	update_force();
	integrate(dt);
	update_lifetime(dt);
	if (is_dead())
		return;
	clean_force();
}
void Particle::integrate(double dt)
{
	if (dt <= 0.0) return; // evitar dt negativo

	switch (integr_mode) {
	case IntegrateMode::EULER:
		int_Euler(dt);
		break;
	case IntegrateMode::EULER_SEMIIMPLICIT:
		int_Euler_Semiimplicit(dt);
		break;
	case IntegrateMode::VERLET:
		int_Verlet(dt);
		break;
	default:
		break;
	}
}
void Particle::update_force()
{
	if (!force.isZero()) {
		acc = CONST_GRAVITY + (force / mass);
	}
}
#pragma endregion
#pragma region metodos de integracion
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
		PxVec3 pos_act = transform.p;

		transform.p = 2.0f * transform.p - last_pos + acc * (t * t);
		vel = (transform.p - last_pos) / (2.0f * float(t));

		calcular_damping(t);

		// actualizamos posición anterior
		last_pos = pos_act;
	}
}

void Particle::calcular_damping(double t)
{
	vel *= pow(damping, t);
}
#pragma endregion


