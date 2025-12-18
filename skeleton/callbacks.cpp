#include "callbacks.hpp"
#include "StructForEntities.h"
#include "SceneManager.h"
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

void ContactReportCallback::onContact(const physx::PxContactPairHeader& pairHeader,
    const physx::PxContactPair* pairs,
    physx::PxU32 nbPairs)
{
    SceneManager::instance().getCurrScene()->handle_contact(pairHeader, pairs, nbPairs);
}