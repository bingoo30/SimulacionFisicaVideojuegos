#pragma once
#include "NormalGenerator.h"
class NormalGeneratorRB :
    public NormalGenerator
{
public:
    NormalGeneratorRB(bool s);
    virtual ~NormalGeneratorRB() = default;
    void setFilter(const physx::PxFilterData& f);
    void setTrigger(bool t);
    Particle_List generate_particles(const Particle_Data& model, const Particle_Deviation_Data& deviation, int n, physx::PxGeometryType::Enum geo, physx::PxMaterial* _mat = nullptr, bool withRender = true) override;
protected:
    bool _static;
    physx::PxFilterData filter;
    bool _isTrigger = false;

};

