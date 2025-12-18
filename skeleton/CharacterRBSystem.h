#pragma once
#include "RigidBodySystem.h"
#include "StructForEntities.h"
#include <unordered_set>

class CharacterRBSystem : public RigidBodySystem, public physx::PxSimulationEventCallback
{
private:
    int moveDirection;
    float moveForce;
    float jumpForce;
    float jumpChargeRate;
    float maxJumpCharge;
    float currentJumpCharge;
    bool isChargingJump;
    bool isGrounded;
    float lastJumpTime;
    float jumpCooldown;
    float currentMoveForce;

    // Para tracking de contactos - solo lectura en callback
    std::unordered_set<physx::PxRigidActor*> groundContacts;

    // Flags para correcciones (se procesan en update, no en callback)
    bool needsGroundCorrection;
    float groundCorrectionY;
    bool needsVelocityResetY;
    bool wasGroundedLastFrame;

public:
    CharacterRBSystem(const Player_Data& pd, const physx::PxVec3& material,
        const physx::PxFilterData& filter);
    ~CharacterRBSystem() = default;

    void init() override;
    void spawn(bool withRender = true, bool isStatic = false) override;
    bool check_out_of_limit(Particle* p) const override;

    // Métodos de control
    void moveLeft();
    void moveRight();
    void stopMoving();
    void startChargingJump();
    void jump();
    void stopChargingJump();

    void update(float dt);

    // PhysX Callbacks
    void onContact(const physx::PxContactPairHeader& pairHeader,
        const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
    void onConstraintBreak(physx::PxConstraintInfo*, physx::PxU32) override {}
    void onWake(physx::PxActor**, physx::PxU32) override {}
    void onSleep(physx::PxActor**, physx::PxU32) override {}
    void onTrigger(physx::PxTriggerPair*, physx::PxU32) override {}
    void onAdvance(const physx::PxRigidBody* const*,
        const physx::PxTransform*, const physx::PxU32) override {}

    // Getters
    bool getIsGrounded() const { return isGrounded; }
    bool getIsChargingJump() const { return isChargingJump; }
    float getCurrentJumpCharge() const { return currentJumpCharge; }
    float getMaxJumpCharge() const { return maxJumpCharge; }

private:
    void checkGroundFromContacts();
    void updateJumpCharge(float dt);
    void applyMovement(float dt);
    void applyLateralDamping(float dt);
    void applyBounds();
    void processGroundCorrections();
};