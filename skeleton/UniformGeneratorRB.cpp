#include "UniformGeneratorRB.h"
#include "StaticRigidBody.h"
#include "DynamicRigidBody.h"
#include "RigidBody.h"
using namespace physx;

UniformGeneratorRB::UniformGeneratorRB(bool s) : _static(s) {}

Particle_List UniformGeneratorRB::generate_particles(const Particle_Data& model, const Particle_Deviation_Data& deviation, int n, physx::PxGeometryType::Enum geo, physx::PxMaterial* _mat, bool withRender)
{
    Particle_List rbs;

    //decidir si queremos generar exactamente n cuerpos nuevos
    int count = n;
    if (deviation.r_cant) count = (int)(round(n * random_fraction()));
    Particle_Data newModel = model;
    for (int i = 0; i < count; ++i) {
        //posicion con desviacion uniforme
        newModel.pos = give_a_new_vec3(model.pos, deviation.ori);

        //velocidad con desviacion uniforme
        newModel.vel = give_a_new_vec3(model.vel, deviation.vel);

        //masa y duracion
        newModel.mass = give_a_new_double(model.mass, deviation.mas);
        newModel.lifetime = give_a_new_double(model.lifetime, deviation.dur);

        //color aleatorio
        newModel.color = deviation.r_color ? colors[model.color_offset, model.color_offset + model.color_tam] : model.color;

        //crear particula y insertar a la lista
        auto g = create_geometry(geo, model.scale);
        PxShape* sh = CreateShape(*g);

        PxFilterData f(0, 0, 0, 0);
        RigidBody* rb = nullptr;
        //miro si es estatico o dynamico
        if (_static) {
            rb = new StaticRigidBody(newModel, f, sh, _mat);
        }
        else rb = new DynamicRigidBody(newModel, f, sh, _mat);
        if (withRender) rb->create_renderItem();
        rbs.push_back(rb);
    }
    return rbs;
}
