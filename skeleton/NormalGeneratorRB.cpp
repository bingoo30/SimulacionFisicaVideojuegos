#include "NormalGeneratorRB.h"
#include "RigidBody.h"
#include "StaticRigidBody.h"
#include "DynamicRigidBody.h"
#include <iostream>
using namespace physx;
NormalGeneratorRB::NormalGeneratorRB(bool s): _static(s)
{
}

Particle_List NormalGeneratorRB::generate_particles(const Particle_Data& model, const Particle_Deviation_Data& deviation, int n, physx::PxGeometryType::Enum geo, physx::PxMaterial* _mat)
{
    Particle_List rbs;

    //decidir si queremos generar exactamente n cuerpos nuevos
    int count = n;
    if (deviation.r_cant) count = (int)(round(n * random_fraction()));
    Particle_Data newModel = model;
    for (int i = 0; i < count; ++i) {
        //posicion con desviacion uniforme
        newModel.pos = give_a_new_vec3(model.pos, deviation.ori, false);

        //velocidad con desviacion uniforme
        newModel.vel = give_a_new_vec3(model.vel, deviation.vel, false);

        //masa y duracion
        newModel.mass = give_a_new_double(model.mass, deviation.mas, false);
        newModel.lifetime = give_a_new_double(model.lifetime, deviation.dur, false);

        //color aleatorio
        newModel.color = deviation.r_color ? colors[random_color_index(model.color_offset, model.color_offset + model.color_tam)] : model.color;

        //crear particula y insertar a la lista
        auto g = create_geometry(geo, PxVec3(model.vol, model.vol, model.vol));
        PxShape* sh = CreateShape(*g);

        PxFilterData f(0, 0, 0, 0);


        RigidBody* rb = nullptr;
        //el tensor se cambia en el sistema!!!
        //miro si es estatico o dynamico
        if (_static) {
            rb = new StaticRigidBody(newModel, f, sh, _mat);
        }
        else {
            rb = new DynamicRigidBody(newModel, f, sh, _mat);
        }

        std::cout << "shape que le paso= " << sh << ", body creado = " << rb->getActor() << std::endl;

        rb->create_renderItem();
        rbs.push_back(rb);
    }
    return rbs;
}
