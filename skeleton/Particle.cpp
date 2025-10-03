#include "Particle.h"
using namespace physx;


#include "Particle.h"
using namespace physx;

Particle::Particle(const PxVec3& p, const PxVec4& color, const PxVec3& v, const PxVec3& a, double d, IntegrateMode t, float m) :
	Entity(p, color, CreateShape(PxSphereGeometry(m))),
	posAnt(p),           // posición anterior = posición inicial
	vel(v),
	acc(a),
	damping(d),
	integrMode(t)
{
}

void Particle::integrate(double t)
{
	if (t <= 0.0) return; // evita dt <= 0

	switch (integrMode) {
	case IntegrateMode::EULER:
		int_Euler(t);
		break;
	case IntegrateMode::EULER_SEMIIMPLICIT:
		int_Euler_Semiimplicit(t);
		break;
	case IntegrateMode::VERLET:
		int_Verlet(t);
		break;
	default:
		break;
	}
}

void Particle::int_Euler(double t)
{
	transform->p += t * vel;
	vel += t * acc;

	calculate_damping(t);
}

void Particle::int_Euler_Semiimplicit(double t)
{
	vel += acc * t;
	//damping
	calculate_damping(t);
	transform->p += t * vel;
}

void Particle::int_Verlet(double t)
{
	// si es la primera integración, usamos Euler
	if (posAnt == transform->p) {
		int_Euler(t);
	}
	else {
		// guardamos posición actual
		PxVec3 posAct = transform->p;

		transform->p = 2.0f * transform->p - posAnt + acc * pow(t,t);
		vel = (transform->p - posAnt) / (2.0f * float(t));

		calculate_damping(t);

		// actualizamos posición anterior
		posAnt = posAct;
	}
}

void Particle::calculate_damping(double t)
{
	vel *= pow(damping, t);
}

