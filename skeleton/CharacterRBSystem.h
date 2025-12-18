#pragma once
#include "RigidBodySystem.h"
#include "StructForEntities.h"
#include <unordered_set>

class CharacterRBSystem : public RigidBodySystem
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

    physx::PxVec3 originalPos;
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
    ~CharacterRBSystem();

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

    void setOriginalPos();
    void reset();

    // PhysX Callbacks
    void onContact(const physx::PxContactPairHeader& pairHeader,
        const physx::PxContactPair* pairs, physx::PxU32 nbPairs);
    void onTrigger(physx::PxTriggerPair*, physx::PxU32) {}

    // Nuevos métodos para manejar contactos específicos
    void onFireContact();
    void onGroundContact(float separation, const physx::PxVec3& normal);
    void onGroundLost();

    // Getters
    bool getIsGrounded() const { return isGrounded; }
    bool getIsChargingJump() const { return isChargingJump; }
    float getCurrentJumpCharge() const { return currentJumpCharge; }
    float getMaxJumpCharge() const { return maxJumpCharge; }

    Particle* getCharacter();

private:
    void checkGroundFromContacts();
    void updateJumpCharge(float dt);
    void applyMovement(float dt);
    void applyLateralDamping(float dt);
    void applyBounds();
    void processGroundCorrections();
};