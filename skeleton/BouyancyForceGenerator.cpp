#include "BouyancyForceGenerator.h"
#include "Particle.h"
using namespace physx;
#include "StructForEntities.h"
BouyancyForceGenerator::BouyancyForceGenerator(float h, float v, float d):ForceGenerator(), height(h), volume(v), liquid_density(d)
{
	Particle_Data pd;
	pd.pos = PxVec3(0.0);

	liquid = new Particle(
		pd.pos,
		pd.color,
		pd.mass,
		CreateShape(physx::PxBoxGeometry(pd.vol * 0.5, pd.vol * 0.5, pd.vol*0.1)),
		pd.vol,
		pd.lifetime,
		pd.vel,
		pd.mode,
		pd.density);
}

BouyancyForceGenerator::~BouyancyForceGenerator()
{
	delete liquid;
}

void BouyancyForceGenerator::register_aux_renders()
{
	liquid->create_renderItem();
}

void BouyancyForceGenerator::derregister_aux_renders()
{
	liquid->derregister_renderItem();
}

physx::PxVec3 BouyancyForceGenerator::calculate_force(Particle* p, double dt)
{
	float h = p->getPosition().y;
	float h0 = liquid->getPosition().y;

	PxVec3 f(0.0f);
	float immersed = 0.0f;
	if (h - h0 > height * 0.5) {
		//todo el cuerpo flotando fuera
		immersed = 0.0f;
	}
	else if (h0 - h > height * 0.5) {
		//cuerpo totalmente sumergido
		immersed = 1.0;
	}
	else {
		immersed = (h0 - h) / height * 0.5;
	}
	f.y = liquid_density * volume * immersed * -CONST_GRAVITY.y;
	return f;
}
