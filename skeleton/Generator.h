#pragma once
#include <random>
#include <list>
#include "Particle.h"
#include "StructForEntities.h"
class Generator {
public:
	Generator(): _mt(std::random_device{}()) {};
	virtual ~Generator() {};

	virtual std::list<Particle*> generate_particles (const Particle_Data& model, const Particle_Deviation_Data& deviation, int n, physx::PxGeometryType::Enum geo) = 0;
   
protected:
	std::mt19937 _mt;
#pragma region metodos auxiliares de calculo
    //funcion auxiliar para distribucion uniforme en rango [-1, 1]
    float uniform_dev(float scale) {
        std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
        return dist(_mt) * scale;
    }

    //funcion auxiliar para distribucion normal con media 0 y desviación 'scale'
    float normal_dev(float scale) {
        std::normal_distribution<float> dist(0.0f, 1.0f);
        return dist(_mt) * scale;
    }

    //funcion auxiliar para color aleatorio entre 0 y max_index
    int random_color_index(int min_index, int max_index) {
        std::uniform_int_distribution<int> dist(min_index, max_index-1);
        return dist(_mt);
    }

    //funcion auxiliar para variacion aleatoria de cantidad
    float random_fraction(float min = 0.5f, float max = 1.0f) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(_mt);
    }

    std::unique_ptr<physx::PxGeometry> create_geometry(physx::PxGeometryType::Enum mode, const physx::PxVec3& size) {
        switch (mode) {
        case physx::PxGeometryType::eSPHERE:
            return std::make_unique<physx::PxSphereGeometry>(size.x);
        case physx::PxGeometryType::eCAPSULE:
            return std::make_unique<physx::PxCapsuleGeometry>(size.x*0.5, size.y*0.25);
        case physx::PxGeometryType::eBOX:
            return std::make_unique<physx::PxBoxGeometry>(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f);
        default:
            return std::make_unique<physx::PxSphereGeometry>(0.1f);
        }
    }

#pragma endregion
};
