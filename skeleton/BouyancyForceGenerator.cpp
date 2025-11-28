#include "BouyancyForceGenerator.h"
#include "Particle.h"
#include "StructForEntities.h"
#include <iostream>
using namespace physx;
BouyancyForceGenerator::BouyancyForceGenerator(float h, float v, float d):ForceGenerator(), height(h), volume(v), liquid_density(d)
{
	Particle_Data pd;
	pd.pos = PxVec3(0.0, 0.0, 0.0);
	pd.density = liquid_density;
	pd.color = PxVec4(0, 0, 1, 0.3);
	pd.lifetime = -1;
	pd.mass = volume*liquid_density;
	pd.mode = NONE;
	liquid = new Particle(
		pd.pos,
		pd.color,
		pd.mass,
		CreateShape(physx::PxBoxGeometry(std::min<double>(height * 10, 10), std::min<double>(height * 4, 10), std::min<double>(height * 5, 10))),
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

	// completamente fuera del agua
	if (h - h0 >= height * 0.5f) {
		immersed = 0.0f;
	}
	// completamente sumergido
	else if (h0 - h >= height * 0.5f) {
		immersed = 1.0f;
	}
	// inmersión parcial
	else {
		immersed = (h0 - h + height * 0.5f) / height;
	}

	f.y = liquid_density * volume * immersed * -CONST_GRAVITY.y;

	//std::cout << "pos particula: " << p->getPosition().y<< "\n";
	return f;
}
