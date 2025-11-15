#include "AnchoredSpringForceGenerator.h"
#include "Particle.h"
#include "StructForEntities.h"
using namespace physx;
AnchoredSpringForceGenerator::AnchoredSpringForceGenerator(double k, double rl, const physx::PxVec3& pos):
	SpringForceGenerator(k,rl, nullptr)
{
	Particle_Data pd;
	other = new Particle(
		pd.pos,
		pd.color,
		pd.mass,
		CreateShape(physx::PxCapsuleGeometry(pd.vol * 0.5, pd.vol * 0.25)),
		pd.vol,
		pd.lifetime,
		pd.vel,
		pd.mode,
		pd.density);
}

AnchoredSpringForceGenerator::~AnchoredSpringForceGenerator()
{
	delete other;
}
