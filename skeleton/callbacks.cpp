#include "callbacks.hpp"
#include "StructForEntities.h"
#include <iostream>
using namespace physx;
extern void onCollision(physx::PxActor* actor1, physx::PxActor* actor2);

physx::PxFilterFlags contactReportFilterShader(
    physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
    physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
    physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
    PX_UNUSED(constantBlock);
    PX_UNUSED(constantBlockSize);

    static int callCount = 0;
    callCount++;

    //// DEBUG EXTENDIDO
    //std::cout << "\n=== FILTER CALL " << callCount << " ===" << std::endl;
    //std::cout << "Actor1 - word0: " << filterData0.word0
    //    << " word1: " << filterData0.word1
    //    << " Type: " << (physx::PxFilterObjectIsTrigger(attributes0) ? "TRIGGER" :
    //        (physx::PxGetFilterObjectType(attributes0) == PxFilterObjectType::eRIGID_DYNAMIC ? "DYNAMIC" : "STATIC")) << std::endl;
    //std::cout << "Actor2 - word0: " << filterData1.word0
    //    << " word1: " << filterData1.word1
    //    << " Type: " << (physx::PxFilterObjectIsTrigger(attributes1) ? "TRIGGER" :
    //        (physx::PxGetFilterObjectType(attributes1) == PxFilterObjectType::eRIGID_DYNAMIC ? "DYNAMIC" : "STATIC")) << std::endl;

    // 1. Triggers primero
    bool isTrigger0 = physx::PxFilterObjectIsTrigger(attributes0);
    bool isTrigger1 = physx::PxFilterObjectIsTrigger(attributes1);

    if (isTrigger0 || isTrigger1) {
        //std::cout << "-> ES TRIGGER, permitiendo (solo notificaciones)" << std::endl;
        pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT
            | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
            | physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
        return physx::PxFilterFlag::eDEFAULT;
    }

    // 2. Filtro por layers
    bool check1 = !(filterData0.word0 & filterData1.word1);
    bool check2 = !(filterData1.word0 & filterData0.word1);

    /*std::cout << "Check1 (A0 & B1): " << filterData0.word0 << " & " << filterData1.word1
        << " = " << (filterData0.word0 & filterData1.word1)
        << " -> " << (check1 ? "FALLA" : "PASA") << std::endl;
    std::cout << "Check2 (B0 & A1): " << filterData1.word0 << " & " << filterData0.word1
        << " = " << (filterData1.word0 & filterData0.word1)
        << " -> " << (check2 ? "FALLA" : "PASA") << std::endl;*/

    if (check1 || check2) {
        //std::cout << "-> SUPRIMIENDO colisión" << std::endl;
        return physx::PxFilterFlag::eSUPPRESS;
    }

    //std::cout << "-> PERMITIENDO colisión física" << std::endl;
    pairFlags = physx::PxPairFlag::eSOLVE_CONTACT
        | physx::PxPairFlag::eDETECT_DISCRETE_CONTACT
        | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
        | physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS
        | physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;

    return physx::PxFilterFlag::eDEFAULT;
}

void ContactReportCallback::onContact(const physx::PxContactPairHeader& pairHeader,
    const physx::PxContactPair* pairs,
    physx::PxU32 nbPairs)
{
    // Ignorar si no es par de actores válido
    if (!pairHeader.actors[0] || !pairHeader.actors[1])
        return;

    // Obtener información de los actores
    PxRigidActor* actor1 = pairHeader.actors[0];
    PxRigidActor* actor2 = pairHeader.actors[1];

    // Obtener filtros para identificar tipos
    PxFilterData filter1, filter2;
    PxShape* shape1, * shape2;

    actor1->getShapes(&shape1, 1);
    actor2->getShapes(&shape2, 1);

    if (shape1) filter1 = shape1->getSimulationFilterData();
    if (shape2) filter2 = shape2->getSimulationFilterData();

    // Identificar tipos por filtros
    std::string type1 = "UNKNOWN";
    std::string type2 = "UNKNOWN";

    if (filter1.word0 == LAYER_GROUND) type1 = "SUELO";
    else if (filter1.word0 == LAYER_PLAYER) type1 = "JUGADOR";
    else if (filter1.word0 == LAYER_FIRE) type1 = "FUEGO";

    if (filter2.word0 == LAYER_GROUND) type2 = "SUELO";
    else if (filter2.word0 == LAYER_PLAYER) type2 = "JUGADOR";
    else if (filter2.word0 == LAYER_FIRE) type2 = "FUEGO";

    // Procesar cada par de contacto
    for (PxU32 i = 0; i < nbPairs; i++) {
        const PxContactPair& cp = pairs[i];

        // Verificar tipo de evento
        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
            std::cout << "CONTACTO INICIADO: " << type1 << " - " << type2 << std::endl;

            // Caso específico: jugador con fuego
            if ((type1 == "JUGADOR" && type2 == "FUEGO") ||
                (type1 == "FUEGO" && type2 == "JUGADOR")) {
                std::cout << "¡¡¡JUGADOR TOCA FUEGO!!! (DAÑO)" << std::endl;

                // Aquí puedes:
                // 1. Reducir vida del jugador
                // 2. Reproducir sonido
                // 3. Activar efectos visuales
            }

            // Caso: jugador con suelo
            if ((type1 == "JUGADOR" && type2 == "SUELO") ||
                (type1 == "SUELO" && type2 == "JUGADOR")) {
                std::cout << "Jugador en suelo" << std::endl;
            }
        }
        else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST) {
            std::cout << "CONTACTO TERMINADO: " << type1 << " - " << type2 << std::endl;
        }
        else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS) {
            // Contacto persistente (cada frame)
            // std::cout << "CONTACTO PERSISTENTE: " << type1 << " - " << type2 << std::endl;
        }
    }
}