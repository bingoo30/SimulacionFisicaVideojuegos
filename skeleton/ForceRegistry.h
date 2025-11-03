#pragma once
#include <vector>
#include <algorithm>
#include "Particle.h"
#include "ForceGenerator.h"
#include <vector>
class ForceRegistry {
public:
#pragma region constructora y destructora
    ForceRegistry();
    virtual ~ForceRegistry();
#pragma endregion
#pragma region metodos publicos
    //agregar un registro
    void add_registry(Particle* particle, ForceGenerator* fg);
    //quitar un generador de fuerza en concreto a una particula
    void remove(Particle* particle, ForceGenerator* fg);
    //quitar todos los generadores de fuerza a una particula
    void clear_particle(Particle* particle);
    //actualizar las fuerzas
    void update_forces();
#pragma endregion
protected:
#pragma region atributos
    struct ForceReg {
        Particle* particle;
        ForceGenerator* fg;
    };
    std::vector<ForceReg> registries;
#pragma endregion
};