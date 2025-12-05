#include "RigidBody.h"
using namespace physx;

RigidBody::RigidBody(Particle_Data& pd, bool _static, PxShape* shape, PxMaterial* material) :
	Particle(pd.pos, pd.color, pd.mass, shape, pd.vol, pd.lifetime, pd.vel, pd.mode, pd.density), is_static(_static)
{
}

void RigidBody::update(double dt)
{
	update_lifetime(dt);
	if (!check_death()) return;
}
