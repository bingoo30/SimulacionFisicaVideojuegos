#include "WindForceGenerator.h"
#include "Particle.h"
using namespace physx;
WindForceGenerator::WindForceGenerator(const physx::PxVec3& v, double _k1, double _k2, double r): vel(v), k1(_k1), k2(_k2), radius(r)
{

}
physx::PxVec3 WindForceGenerator::calculate_force(Particle* p)
{
	//velocidad de la particula
	PxVec3 p_vel = p->getVel();
	//Fv = 𝑘1(V𝑣 − 𝑣) + 𝑘2‖V𝑣 −𝑣‖(V𝑣 −𝑣)
	//diferencia de velocidad
	PxVec3 dv = vel - p_vel;
	return (k1 * dv + k2 * dv.magnitude()*dv);
}

void WindForceGenerator::update_force(Particle* p)
{
	if (p != nullptr) {
		p->add_force(calculate_force(p));
	}
}
