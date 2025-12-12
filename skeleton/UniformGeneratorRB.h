#pragma once

// este generador se usa para generar RigidBodies

#include "UniformGenerator.h"
class UniformGeneratorRB :
    public UniformGenerator
{
public:
    UniformGeneratorRB(bool s);
    virtual ~UniformGeneratorRB() = default;
    Particle_List generate_particles(const Particle_Data& model, const Particle_Deviation_Data& deviation, int n, physx::PxGeometryType::Enum geo, physx::PxMaterial* _mat=nullptr, bool withRender = true) override;
protected:
    bool _static;

};

