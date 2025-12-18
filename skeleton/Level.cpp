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
#include "RubberBandForceGenerator.h"
#include "RainParticleSystem.h"
#include "SceneManager.h"
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
    //// DEBUG: Verificar filtros aplicados
    //std::cout << "\n=== DEBUG FILTER DATA ===" << std::endl;
    //std::cout << "Player filter: word0=" << playerFilterData.word0
    //    << " word1=" << playerFilterData.word1 << std::endl;
    //std::cout << "Ground filter: word0=" << groundFilterData.word0
    //    << " word1=" << groundFilterData.word1 << std::endl;
    //std::cout << "Fire filter: word0=" << trampFilterData.word0
    //    << " word1=" << trampFilterData.word1 << std::endl;
    //std::cout << "Key filter: word0=" << keyFilterData.word0
    //    << " word1=" << keyFilterData.word1 << std::endl;
    //std::cout << "Door filter: word0=" << doorFilterData.word0
    //    << " word1=" << doorFilterData.word1 << std::endl;

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
    Wind_Data wd;
    f >> x;

    Rain_Particle_Data rpd; 
    rpd.pos = { x, 80, -10 };
    Rain_Deviation_Data rdd; rdd.ori.x = rdd.ori.y = 100;

    Rain_Particle_Data spd = rpd; 
    spd.color = { 1.0,1.0,1.0,0.7 };
    Rain_Deviation_Data sdd = rdd;

    rpd.scale = {0.1, 0.1, 0.1};
    auto g = new GroundSystem(gd, 1, groundFilterData);
    g->init();
    auto& _gd = g->getModel();
    add_RB_system(g);

    character = new CharacterRBSystem(pd, characterMaterial, playerFilterData);
    auto& _pd = character->getModel();
    character->init();
    add_RB_system(character);

    auto gDoor = new GroundSystem(dd, 1, doorFilterData, PxVec3(0.0));
    gDoor->init();
    auto& _dd = gDoor->getModel();
    add_RB_system(gDoor);

    auto k = new KeyRBSystem(kd, keyMaterial, keyFilterData);
    k->init();
    auto& _kd = k->getModel();
    add_RB_system(k);
    key = k;

    char c; 
    int index;
    FireRBSystem* fr = nullptr;
    WindForceGenerator* wind = nullptr;
    RainParticleSystem* rain = nullptr;
    RainParticleSystem* snow = nullptr;
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
            character->setOriginalPos();
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
            key->setInitPos(_kd.pos);
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
            //pos y area del viento
            f >> wd.center.x >> wd.center.y >> wd.area;
            f >> wd.vel.x >> wd.vel.y;
            f >> wd.k1;
            wind = new WindForceGenerator(wd.center, wd.vel, wd.area, wd.k1, wd.dragCoef, false);
            //solo a los que quiero aplicarles
            f >> c;
            if (c == 'p')
                character->add_force_generator(wind);
            else if (c == 'k')
                key->add_force_generator(wind);
            break;
        case 'x':
            f >> c;
            int n;
            f >> n;
            switch (c)
            {
            case 's':
                snow = new RainParticleSystem(spd, sdd, n);
                snow->init();
                add_particle_system(snow);
                break;
            case 'r':
                rain = new RainParticleSystem(rpd, rdd, n);
                rain->init();
                add_particle_system(rain);
                break;
            default:

                break;
            }
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
        character->stopChargingJump();
        character->jump();
    }
}
void Level::handle_contact(const physx::PxContactPairHeader& pairHeader,
    const physx::PxContactPair* pairs,
    physx::PxU32 nbPairs)
{
    // Ignorar si no es par de actores válido o no tenemos jugador
    if (!pairHeader.actors[0] || !pairHeader.actors[1] || !character)
        return;

    // Obtener información de los actores
    PxRigidActor* actor1 = pairHeader.actors[0];
    PxRigidActor* actor2 = pairHeader.actors[1];

    // Obtener filtros
    PxFilterData filter1, filter2;
    PxShape* shape1, * shape2;

    actor1->getShapes(&shape1, 1);
    actor2->getShapes(&shape2, 1);

    if (shape1) filter1 = shape1->getSimulationFilterData();
    if (shape2) filter2 = shape2->getSimulationFilterData();

    // Identificar tipos
    bool isPlayer1 = (filter1.word0 & LAYER_PLAYER) != 0;
    bool isPlayer2 = (filter2.word0 & LAYER_PLAYER) != 0;
    bool isGround1 = (filter1.word0 & LAYER_GROUND) != 0;
    bool isGround2 = (filter2.word0 & LAYER_GROUND) != 0;
    bool isFire1 = (filter1.word0 & LAYER_FIRE) != 0;
    bool isFire2 = (filter2.word0 & LAYER_FIRE) != 0;
    bool isKey1 = (filter1.word0 & LAYER_KEY) != 0;
    bool isKey2 = (filter2.word0 & LAYER_KEY) != 0;
    bool isDoor1 = (filter1.word0 & LAYER_DOOR) != 0;
    bool isDoor2 = (filter2.word0 & LAYER_DOOR) != 0;

    // Procesar cada par de contacto
    for (PxU32 i = 0; i < nbPairs; i++) {
        const PxContactPair& cp = pairs[i];

        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
            // JUGADOR - FUEGO
            if ((isPlayer1 && isFire2) || (isPlayer2 && isFire1)) {
                //std::cout << "¡¡¡JUGADOR TOCA FUEGO!!!" << std::endl;
                if (character) {
                    character->onFireContact();
                }
                SceneManager::instance().change_scene(DEFEAT);
            }

            // JUGADOR - SUELO
            else if ((isPlayer1 && isGround2) || (isPlayer2 && isGround1)) {
                //std::cout << "Jugador toca suelo" << std::endl;

                // Extraer información del contacto para correcciones
                PxContactPairPoint contactPoints[32];
                PxU32 numContacts = cp.extractContacts(contactPoints, 32);

                for (PxU32 j = 0; j < numContacts; j++) {
                    const PxContactPairPoint& point = contactPoints[j];

                    // Notificar al jugador sobre el contacto con suelo
                    if (character && point.normal.y > 0.7f) {
                        character->onGroundContact(point.separation, point.normal);
                    }
                }
            }

            // JUGADOR - LLAVE
            else if ((isPlayer1 && isKey2) || (isPlayer2 && isKey1)) {
                std::cout << "¡¡¡JUGADOR RECOGE LLAVE!!!" << std::endl;

                muelle->set_alive(false);
            }

            // LLAVE - PUERTA
            else if ((isKey1 && isDoor2) || (isKey2 && isDoor1)) {
                std::cout << "Llave toca puerta" << std::endl;
                SceneManager::instance().change_scene(VICTORY);
            }
        }

        else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST) {
            // JUGADOR - SUELO (contacto terminado)
            if ((isPlayer1 && isGround2) || (isPlayer2 && isGround1)) {
                //std::cout << "Jugador deja de tocar suelo" << std::endl;

                if (character) {
                    character->onGroundLost();
                }
            }
        }
    }
}
void Level::update(double dt)
{
    Scene::update(dt);
    character->update(dt);
}

void Level::enter()
{
    Scene::enter();
    Camera* c = GetCamera();
    c->setCamera(x);
}

void Level::exit()
{
    Scene::exit();
    character->reset();
    key->reset();
    muelle->set_alive(true);
    Camera* c = GetCamera();
    c->resetCamera();
}