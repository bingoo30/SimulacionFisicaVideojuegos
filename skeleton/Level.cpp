#include "Level.h"
#include <iostream>
#include <fstream>
#include "GroundSystem.h"
#include "FireParticleSystem.h"
#include "FireRBSystem.h"
#include "CharacterRBSystem.h"
#include "WindForceGenerator.h"
/*
* Estructura fichero nivel: <char> <x> <y> <color_index> <scale_x> <scale_y> <scale_z>
* acaba la textura cuando no queda cosas por leer
*/
#include <iostream>
using namespace physx;
using namespace std;

Level::Level(int i): file("txt/level"+std::to_string(i)+".txt")
{
    // LAYER_PLAYER = 1, LAYER_FIRE = 2, LAYER_GROUND = 4

    // Suelo: soy GROUND (4), colisiono con PLAYER (1)
    groundFilterData = PxFilterData(
        LAYER_GROUND,    // 4 (soy suelo)
        LAYER_PLAYER,    // 1 (colisiono con jugador)
        0, 0
    );

    // Jugador: soy PLAYER (1), colisiono con GROUND (4) y FIRE (2)
    playerFilterData = PxFilterData(
        LAYER_PLAYER,                      // 1 (soy jugador)
        LAYER_GROUND | LAYER_FIRE,         // 4 | 2 = 6 (colisiono con suelo y fuego)
        0, 0
    );

    // Fuego: soy FIRE (2), colisiono con PLAYER (1)
    trampFilterData = PxFilterData(
        LAYER_FIRE,        // 2 (soy fuego)
        LAYER_PLAYER,      // 1 (colisiono con jugador)
        0, 0
    );
}

void Level::init()
{
    std::ifstream f(file);
    if (!f.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << file << "\n";
        return;
    }

    Ground_Data gd; gd.lifetime = -1;
    Player_Data pd; pd.lifetime = -1;
    Fire_Particle_Data fpd; Fire_Deviation_Data fdd;

    auto g = new GroundSystem(gd, 1, groundFilterData);
    g->init();
    auto& _gd = g->getModel();
    add_RB_system(g);

    character = new CharacterRBSystem(pd, characterMaterial, playerFilterData);
    auto& _pd = character->getModel();
    character->init();
    add_RB_system(character);

    
    char c; 
    int index;
    FireRBSystem* fr = nullptr;
    while (f >> c) {
        switch (tolower(c))
        {
        //suelo
        case 'g':
            //lee la posicion
            f >> _gd.pos.x >> _gd.pos.y;
            //lee el color
            f >> index; _gd.color = colors[index];
            f >> _gd.scale.x >> _gd.scale.y >> _gd.scale.z;
            //indice de material
            f >> index;
            g->setMaterial(materials[index]);
            g->spawn(false, true);
            break;
        //trampa
        case 't':
            f >> fpd.pos.x >> fpd.pos.y;
            //offset por donde empieza a coger el color, aqui son varios colores para un generador
            f >> fpd.color_offset;
            f >> fpd.scale.x >> fpd.scale.y >> fpd.scale.z;

            fr = new FireRBSystem(fpd, fdd, 5, nullMaterial, trampFilterData);
            fr->init();
            add_particle_system(fr);
            break;
        //personaje
        case 'p':
            //lee la posicion
            f >> _pd.pos.x >> _pd.pos.y;
            //lee el color
            index; f >> index; _pd.color = colors[index];
            character->spawn(false, false);
            break;
        //llave
        case 'k':
            break;
        //puerta
        case 'd':
            break;
        //fuerza
        case 'f':
            break;
        default:
            break;
        }
    }
    f.close();
}

void Level::handle_special_input(int key)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        character->moveLeft();
        break;
    case GLUT_KEY_RIGHT:
        character->moveRight();
        break;
    }
}

void Level::handle_input(unsigned char key)
{
}

void Level::update(double dt)
{
    Scene::update(dt);
    character->update(dt);
}
