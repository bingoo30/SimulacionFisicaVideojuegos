#include "callbacks.hpp"
#include "StructForEntities.h"
#include <iostream>

using namespace physx;

// FilterShader (tu versión sin el 'extern')
physx::PxFilterFlags contactReportFilterShader(
    physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
    physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
    physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
    PX_UNUSED(constantBlock);
    PX_UNUSED(constantBlockSize);

    // 1. Triggers primero
    bool isTrigger0 = physx::PxFilterObjectIsTrigger(attributes0);
    bool isTrigger1 = physx::PxFilterObjectIsTrigger(attributes1);

    if (isTrigger0 || isTrigger1) {
        pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT
            | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
            | physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
        return physx::PxFilterFlag::eDEFAULT;
    }

    // 2. Filtro por layers
    bool canCollide1 = (filterData0.word0 & filterData1.word1) != 0;
    bool canCollide2 = (filterData1.word0 & filterData0.word1) != 0;

    // Si AMBOS quieren colisionar, permitimos
    if (canCollide1 && canCollide2) {
        pairFlags = physx::PxPairFlag::eSOLVE_CONTACT
            | physx::PxPairFlag::eDETECT_DISCRETE_CONTACT
            | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
            | physx::PxPairFlag::eNOTIFY_TOUCH_LOST
            | physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS
            | physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;
        return physx::PxFilterFlag::eDEFAULT;
    }
    else {
        return physx::PxFilterFlag::eSUPPRESS;
    }
}

// Implementación CORRECTA de onContact
void ContactReportCallback::onContact(const physx::PxContactPairHeader& pairHeader,
    const physx::PxContactPair* pairs,
    physx::PxU32 nbPairs)
{
    std::cout << "\n=== ONCONTENT LLAMADO ===" << std::endl;
    std::cout << "nbPairs: " << nbPairs << std::endl;

    // Ignorar si no es par de actores válido
    if (!pairHeader.actors[0] || !pairHeader.actors[1])
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
    std::string type1 = "UNKNOWN";
    std::string type2 = "UNKNOWN";

    if ((filter1.word0 & LAYER_GROUND) != 0) type1 = "SUELO";
    else if ((filter1.word0 & LAYER_PLAYER) != 0) type1 = "JUGADOR";
    else if ((filter1.word0 & LAYER_FIRE) != 0) type1 = "FUEGO";

    if ((filter2.word0 & LAYER_GROUND) != 0) type2 = "SUELO";
    else if ((filter2.word0 & LAYER_PLAYER) != 0) type2 = "JUGADOR";
    else if ((filter2.word0 & LAYER_FIRE) != 0) type2 = "FUEGO";

    std::cout << "Tipo Actor1: " << type1 << std::endl;
    std::cout << "Tipo Actor2: " << type2 << std::endl;

    // Procesar cada par de contacto
    for (PxU32 i = 0; i < nbPairs; i++) {
        const PxContactPair& cp = pairs[i];

        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
            std::cout << "CONTACTO INICIADO entre " << type1 << " y " << type2 << std::endl;

            if ((type1 == "JUGADOR" && type2 == "FUEGO") ||
                (type1 == "FUEGO" && type2 == "JUGADOR")) {
                std::cout << "¡¡¡JUGADOR TOCA FUEGO!!!" << std::endl;
            }
        }
        else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST) {
            std::cout << "CONTACTO TERMINADO entre " << type1 << " y " << type2 << std::endl;
        }
    }
}