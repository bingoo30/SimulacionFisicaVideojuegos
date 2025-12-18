#include "Level.h"
#include <iostream>
#include <fstream>
#include "GroundSystem.h"
#include "FireParticleSystem.h"
#include "FireRBSystem.h"
#include "CharacterRBSystem.h"
#include "WindForceGenerator.h"
#include "SpringForceGenerator.h"
#include "KeyRBSystem.h"
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

    // Suelo: soy GROUND (4), colisiono con PLAYER (1) y KEY (8)
    groundFilterData = PxFilterData(
        LAYER_GROUND,    // 4 (soy suelo)
        LAYER_PLAYER | LAYER_KEY,    // 1 | 8 = 9 (colisiono con jugador y la llave)
        0, 0
    );

    // Jugador: soy PLAYER (1), colisiono con GROUND (4), KEY (8) y FIRE (2)
    playerFilterData = PxFilterData(
        LAYER_PLAYER,                      // 1 (soy jugador)
        LAYER_GROUND | LAYER_FIRE | LAYER_KEY,         // 4 | 2 | 8 = 14 (colisiono con suelo y fuego)
        0, 0
    );

    // Fuego: soy FIRE (2), colisiono con PLAYER (1)
    trampFilterData = PxFilterData(
        LAYER_FIRE,        // 2 (soy fuego)
        LAYER_PLAYER,      // 1 (colisiono con jugador)
        0, 0
    );

    // Puerta: soy DOOR (16), colisiono con KEY (8)
    doorFilterData = PxFilterData(
        LAYER_DOOR,        // 16 (soy puerta)
        LAYER_KEY,      // 8 (colisiono con llave) 
        0, 0
    );

    // Llave: soy KEY (8), colisiono con DOOR (16) y PLAYER (1)
    keyFilterData = PxFilterData(
        LAYER_KEY,        // 8 (soy llave)
        LAYER_DOOR |LAYER_PLAYER | LAYER_GROUND,      // 16 | 1 | 4 = 21 (colisiono con puerta, suelo y jugador) 
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

    Ground_Data gd; Ground_Data dd;
    Player_Data pd; 
    Fire_Particle_Data fpd; Fire_Deviation_Data fdd;
    Key_Data kd;
    Spring_Data sd;

    auto g = new GroundSystem(gd, 1, groundFilterData);
    g->init();
    auto& _gd = g->getModel();
    add_RB_system(g);

    character = new CharacterRBSystem(pd, characterMaterial, playerFilterData);
    auto& _pd = character->getModel();
    character->init();
    add_RB_system(character);

    auto gDoor = new GroundSystem(dd, 1, doorFilterData, PxVec3(0.0), true);
    gDoor->init();
    auto& _dd = gDoor->getModel();
    add_RB_system(gDoor);

    auto k = new KeyRBSystem(kd, keyMaterial, keyFilterData);
    k->init();
    auto& _kd = k->getModel();
    add_RB_system(k);

    char c; 
    int index;
    FireRBSystem* fr = nullptr;
    SpringForceGenerator* muelle = nullptr;
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
            //lee la posicion
            f >> _kd.pos.x >> _kd.pos.y;
            //lee el color
            index; f >> index; _kd.color = colors[index];
            k->spawn(false, false);

            //creo la particula para el muelle
            _gd.pos = _kd.pos;
            _gd.pos.y += 20;
            _gd.scale = PxVec3(2);
            //lectura color de la particula del muelle
            f >> index; _gd.color = colors[index];
            //lectura parametros del muelle
            f >> sd.k >> sd.resisting_length;
            muelle = new SpringForceGenerator(sd.k, sd.resisting_length, create_Platform(_gd));
            k->add_force_generator(muelle);
            break;
        //puerta
        case 'd':
            //lee la posicion
            f >> _dd.pos.x >> _dd.pos.y;
            //lee el color
            f >> index; _dd.color = colors[index];
            //lee la escala
            f >> _dd.scale.x >> _dd.scale.y >> _dd.scale.z;
            //indice de material
            f >> index;
            gDoor->setMaterial(materials[index]);
            gDoor->spawn(false, true);
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

void Level::handle_special_key_up(int key)
{
    // Manejar liberación de teclas especiales
    switch (key)
    {
    case GLUT_KEY_LEFT:
        // Si se suelta flecha izquierda
        if (character)
            character->stopMoving();  // O lógica específica
        break;

    case GLUT_KEY_RIGHT:
        // Si se suelta flecha derecha
        if (character)
            character->stopMoving();  // O lógica específica
        break;
    }
}

void Level::handle_input(unsigned char key)
{
    switch (toupper(key))
    {
    case ' ':
        if (character)
            character->startChargingJump();
        break;
    }
}
void Level::handle_key_up(unsigned char key)
{
    if (key == ' ' && character)
    {
        // Salto
        character->startChargingJump();
        character->jump();
    }
}
void Level::update(double dt)
{
    Scene::update(dt);
    character->update(dt);
}
