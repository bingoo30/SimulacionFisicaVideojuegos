#include "Particle.h"
using namespace physx;


#include "Particle.h"
using namespace physx;

Particle::Particle(const PxVec3& p, const PxVec4& color, const PxVec3& v, const PxVec3& a, double d, IntegrateMode t, float m, double lt, double vol) :
	Entity(p, color, CreateShape(PxSphereGeometry(vol)), lt, m),
	pos_ant(p),           // posición anterior = posición inicial
	vel(v),
	acc(a),
	damping(d),
	integr_mode(t)
{
}

void Particle::integrate(double t)
{
	if (t <= 0.0) return; // evitar dt negativo

	switch (integr_mode) {
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
	 
	calcular_damping(t);
}

void Particle::int_Euler_Semiimplicit(double t)
{
	vel += acc * t;
	//damping
	calcular_damping(t);
	transform->p += t * vel;
}

void Particle::int_Verlet(double t)
{
	// si es la primera integración, usamos Euler
	if (pos_ant == transform->p) {
		int_Euler(t);
	}
	else {
		// guardamos posición actual
		PxVec3 posAct = transform->p;

		transform->p = 2.0f * transform->p - pos_ant + acc * (t*t);
		vel = (transform->p - pos_ant) / (2.0f * float(t));

		calcular_damping(t);

		// actualizamos posición anterior
		pos_ant = posAct;
	}
}

void Particle::calcular_damping(double t)
{
	vel *= pow(damping, t);
}


