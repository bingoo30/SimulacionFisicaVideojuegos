#include "CharacterRBSystem.h"
#include "UniformGeneratorRB.h"
#include "SceneManager.h"
#include <iostream>
#include <algorithm>

using namespace physx;

extern PxPhysics* gPhysics;
extern PxScene* gScene;

CharacterRBSystem::CharacterRBSystem(const Player_Data& pd, const PxVec3& material, const PxFilterData& filter)
    : RigidBodySystem(pd, Particle_Deviation_Data(), 1, PxGeometryType::eBOX, false, material, filter),
    moveDirection(0), moveForce(500.0f), jumpForce(800.0f),
    jumpChargeRate(500.0f), maxJumpCharge(2000.0f), currentJumpCharge(0.0f),
    isChargingJump(false), isGrounded(false),
    lastJumpTime(0.0f), jumpCooldown(0.3f), currentMoveForce(0.0f),
    needsGroundCorrection(false), groundCorrectionY(0.0f),
    needsVelocityResetY(false), wasGroundedLastFrame(false),
    originalPos(pd.pos), accumulatedTime(0.0f), groundTimer(0.0f)
{
}

CharacterRBSystem::~CharacterRBSystem()
{
}

void CharacterRBSystem::init()
{
    spawn_acu = -1;
    auto g = new UniformGeneratorRB(false);
    g->setFilter(this->filter);
    add_generator(g);
}

void CharacterRBSystem::spawn(bool withRender, bool isStatic)
{
    for (auto g : generators) {
        auto new_particles = g->generate_particles(model, deviation, num, geometry, mat, withRender);
        for (auto& new_p : new_particles) {
            if (!isStatic) {
                for (auto& fg : force_generators) {
                    local_registry.add_registry(new_p, fg.get());
                }
            }
            auto p = static_cast<DynamicRigidBody*>(new_p);
            PxRigidDynamic* dyn = static_cast<PxRigidDynamic*>(p->getActor());

            dyn->setActorFlag(PxActorFlag::eSEND_SLEEP_NOTIFIES, true);
            dyn->setName("PlayerCharacter");
            dyn->setAngularDamping(5.0f);

            dyn->setRigidDynamicLockFlags(
                PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
                PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y |
                PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z |
                PxRigidDynamicLockFlag::eLOCK_LINEAR_Z
            );

            particles_list.push_back(std::unique_ptr<Particle>(new_p));
        }
        new_particles.clear();
    }
}

void CharacterRBSystem::onFireContact()
{
    reset();
    SceneManager::instance().change_scene(INTRO);
}

void CharacterRBSystem::onGroundContact(float separation, const PxVec3& normal)
{
    if (normal.y > 0.7f) {
        isGrounded = true;
        wasGroundedLastFrame = true;
        groundTimer = 0.0f;

        if (separation < -0.05f) {
            needsGroundCorrection = true;

            if (!particles_list.empty()) {
                auto* playerRB = static_cast<DynamicRigidBody*>(particles_list.front().get());
                PxRigidDynamic* actor = static_cast<PxRigidDynamic*>(playerRB->getActor());

                if (actor) {
                    PxTransform pose = actor->getGlobalPose();
                    groundCorrectionY = pose.p.y - separation;
                }
            }
        }
    }
}

void CharacterRBSystem::onGroundLost()
{
    isGrounded = false;
}

void CharacterRBSystem::moveLeft() {
    moveDirection = -1;
}

void CharacterRBSystem::moveRight() {
    moveDirection = 1;
}

void CharacterRBSystem::stopMoving() {
    moveDirection = 0;
    currentMoveForce = 0.0f;
}

void CharacterRBSystem::startChargingJump()
{
    if (isGrounded) {
        isChargingJump = true;
    }
}

void CharacterRBSystem::stopChargingJump()
{
    isChargingJump = false;
}

void CharacterRBSystem::jump()
{
    if (particles_list.empty()) return;

    if (lastJumpTime > 0.0f) {
        return;
    }

    if (!isGrounded) {
        return;
    }

    auto* playerRB = static_cast<DynamicRigidBody*>(particles_list.front().get());
    PxRigidDynamic* actor = static_cast<PxRigidDynamic*>(playerRB->getActor());

    if (!actor) return;

    float finalJumpForce = jumpForce + currentJumpCharge;

    actor->addForce(PxVec3(0.0f, finalJumpForce, 0.0f), PxForceMode::eIMPULSE);

    currentJumpCharge = 0.0f;
    isChargingJump = false;
    isGrounded = false;
    wasGroundedLastFrame = true;
    groundTimer = 0.0f;

    lastJumpTime = jumpCooldown;
}

Particle* CharacterRBSystem::getCharacter()
{
    if (particles_list.empty()) return nullptr;
    else return particles_list.front().get();
}

void CharacterRBSystem::processGroundCorrections()
{
    if (particles_list.empty()) return;

    auto* playerRB = static_cast<DynamicRigidBody*>(particles_list.front().get());
    PxRigidDynamic* actor = static_cast<PxRigidDynamic*>(playerRB->getActor());

    if (!actor) return;

    if (needsGroundCorrection) {
        PxTransform pose = actor->getGlobalPose();
        pose.p.y = groundCorrectionY;
        actor->setGlobalPose(pose);
        needsGroundCorrection = false;

        PxVec3 vel = actor->getLinearVelocity();
        if (vel.y < 0) {
            vel.y = 0.0f;
            actor->setLinearVelocity(vel);
        }
    }
}

void CharacterRBSystem::update(float dt)
{
    accumulatedTime += dt;

    if (lastJumpTime > 0.0f) {
        lastJumpTime -= dt;
        if (lastJumpTime < 0.0f) lastJumpTime = 0.0f;
    }

    if (isGrounded) {
        groundTimer = 0.0f;
    }
    else {
        groundTimer += dt;
        if (groundTimer > 0.2f) {
            wasGroundedLastFrame = false;
        }
    }

    if (particles_list.empty()) return;

    updateJumpCharge(dt);
    processGroundCorrections();
    applyMovement(dt);
    applyLateralDamping(dt);
    applyBounds();
}

void CharacterRBSystem::setOriginalPos()
{
    originalPos = model.pos;
}

void CharacterRBSystem::reset()
{
    if (particles_list.empty()) return;

    auto* playerRB = static_cast<DynamicRigidBody*>(particles_list.front().get());
    PxRigidDynamic* actor = static_cast<PxRigidDynamic*>(playerRB->getActor());

    if (!actor) return;

    PxTransform originalTransform(originalPos);
    actor->setGlobalPose(originalTransform);

    actor->setLinearVelocity(PxVec3(0, 0, 0));
    actor->setAngularVelocity(PxVec3(0, 0, 0));

    moveDirection = 0;
    currentMoveForce = 0.0f;
    currentJumpCharge = 0.0f;
    isChargingJump = false;
    isGrounded = false;
    wasGroundedLastFrame = false;

    needsGroundCorrection = false;
    needsVelocityResetY = false;
    groundCorrectionY = 0.0f;
    groundTimer = 0.0f;
}

void CharacterRBSystem::updateJumpCharge(float dt)
{
    if (isChargingJump && isGrounded) {
        currentJumpCharge = std::min<float>(currentJumpCharge + jumpChargeRate * dt, maxJumpCharge);
    }
}

void CharacterRBSystem::applyMovement(float dt)
{
    if (particles_list.empty()) return;

    auto* playerRB = static_cast<DynamicRigidBody*>(particles_list.front().get());
    PxRigidDynamic* actor = static_cast<PxRigidDynamic*>(playerRB->getActor());

    if (!actor) return;

    PxVec3 currentVelocity = actor->getLinearVelocity();
    float mass = actor->getMass();

    if (moveDirection != 0) {
        float targetForce = moveDirection * moveForce;

        float acceleration = 15.0f;
        float forceDelta = targetForce - currentMoveForce;
        currentMoveForce += forceDelta * std::min<float>(acceleration * dt, 1.0f);

        PxVec3 movementForce(currentMoveForce, 0.0f, 0.0f);
        actor->addForce(movementForce, PxForceMode::eFORCE);

        float maxHorizontalSpeed = 15.0f;
        if (std::abs(currentVelocity.x) > maxHorizontalSpeed) {
            currentVelocity.x = (currentVelocity.x > 0) ? maxHorizontalSpeed : -maxHorizontalSpeed;
            actor->setLinearVelocity(currentVelocity);
        }
    }
    else {
        float deceleration = 8.0f;
        currentMoveForce *= (1.0f - std::min<float>(deceleration * dt, 1.0f));

        if (std::abs(currentMoveForce) < 0.5f) {
            currentMoveForce = 0.0f;
        }

        if (isGrounded && std::abs(currentVelocity.x) > 0.1f) {
            float frictionForce = -currentVelocity.x * mass * 5.0f;
            actor->addForce(PxVec3(frictionForce, 0.0f, 0.0f), PxForceMode::eFORCE);

            if (std::abs(currentVelocity.x) < 0.2f) {
                currentVelocity.x = 0.0f;
                actor->setLinearVelocity(currentVelocity);
            }
        }
    }
}

void CharacterRBSystem::applyLateralDamping(float dt)
{
    if (particles_list.empty()) return;

    auto* playerRB = static_cast<DynamicRigidBody*>(particles_list.front().get());
    PxRigidDynamic* actor = static_cast<PxRigidDynamic*>(playerRB->getActor());

    if (!actor) return;

    PxVec3 velocity = actor->getLinearVelocity();
    float mass = actor->getMass();

    if (!isGrounded) {
        float airControlFactor = 0.3f;

        if (moveDirection != 0 && std::abs(velocity.x) < 8.0f) {
            float airForce = moveDirection * moveForce * airControlFactor;
            actor->addForce(PxVec3(airForce, 0.0f, 0.0f), PxForceMode::eFORCE);
        }
        else if (std::abs(velocity.x) > 0.01f) {
            float airDamping = 1.5f;
            float dampingForce = -velocity.x * mass * airDamping * airControlFactor;
            actor->addForce(PxVec3(dampingForce, 0.0f, 0.0f), PxForceMode::eFORCE);
        }
    }
}

void CharacterRBSystem::applyBounds()
{
    if (particles_list.empty()) return;

    auto* playerRB = static_cast<DynamicRigidBody*>(particles_list.front().get());
    PxRigidDynamic* actor = static_cast<PxRigidDynamic*>(playerRB->getActor());

    if (!actor) return;

    PxTransform pose = actor->getGlobalPose();
    PxVec3 velocity = actor->getLinearVelocity();

    float maxHeight = 200.0f;
    if (pose.p.y > maxHeight) {
        velocity.y = -5.0f;
        velocity.x *= 0.7f;
        velocity.z *= 0.7f;
        actor->setLinearVelocity(velocity);
    }
}

bool CharacterRBSystem::check_out_of_limit(Particle* p) const
{
    auto rb = static_cast<DynamicRigidBody*>(p);
    bool isOut = rb->getPosition().y < -10.0f;

    if (isOut) {
        SceneManager::instance().change_scene(DEFEAT);
        std::cout << "Jugador cayó fuera del mapa!" << std::endl;
    }

    return false;
}