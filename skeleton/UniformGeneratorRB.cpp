#include "UniformGeneratorRB.h"
#include "StaticRigidBody.h"
#include "DynamicRigidBody.h"
#include "RigidBody.h"
using namespace physx;

UniformGeneratorRB::UniformGeneratorRB(bool s) : _static(s) {}

Particle_List UniformGeneratorRB::generate_particles(const Particle_Data& model, const Particle_Deviation_Data& deviation, int n, physx::PxGeometryType::Enum geo, physx::PxMaterial* _mat)
{
    Particle_List rbs;

    //decidir si queremos generar exactamente n cuerpos nuevos
    int count = n;
    if (deviation.r_cant) count = (int)(round(n * random_fraction()));

    for (int i = 0; i < count; ++i) {
        //posicion con desviacion uniforme
        PxVec3 pos = give_a_new_vec3(model.pos, deviation.ori);

        //velocidad con desviacion uniforme
        PxVec3 vel = give_a_new_vec3(model.vel, deviation.vel);

        //masa y duracion
        double mass = give_a_new_double(model.mass, deviation.mas);
        double life = give_a_new_double(model.lifetime, deviation.dur);

        //color aleatorio
        PxVec4 color = deviation.r_color ? colors[model.color_offset, model.color_offset + model.color_tam] : model.color;

        //crear particula y insertar a la lista
        auto g = create_geometry(geo, PxVec3(model.vol, model.vol, model.vol));
        PxShape* sh = CreateShape(*g);
        
        PxFilterData f(0, 0, 0, 0);
        RigidBody* rb = nullptr;
        //miro si es estatico o dynamico
        if (_static) {
            rb = new StaticRigidBody(model, f, sh, _mat);
        }
        else rb = new DynamicRigidBody(model, f, sh, _mat);

        rb->create_renderItem();
        rbs.push_back(rb);
}
