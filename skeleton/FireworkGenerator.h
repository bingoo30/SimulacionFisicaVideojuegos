#pragma once
#include "Generator.h"
class FireworkGenerator :
	public Generator
{
public:
	FireworkGenerator() = default;
	virtual ~FireworkGenerator() = default;

	Particle_List generate_particles(const Particle_Data& model, const Particle_Deviation_Data& deviation, int n, physx::PxGeometryType::Enum geo) override;
};
