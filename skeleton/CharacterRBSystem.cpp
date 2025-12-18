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
    needsVelocityResetY(false), wasGroundedLastFrame(false)
{
    // Registrar este sistema como callback de colisión
    if (gScene) {
        gScene->setSimulationEventCallback(this);
    }
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

            // Configurar para recibir notificaciones de contacto
            dyn->setActorFlag(PxActorFlag::eSEND_SLEEP_NOTIFIES, true);
            dyn->setName("PlayerCharacter");
            dyn->setAngularDamping(5.0f);

            // Bloquear rotaciones y movimiento en Z
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

// Callback de contacto de PhysX - SOLO LECTURA
void CharacterRBSystem::onContact(const PxContactPairHeader& pairHeader,
    const PxContactPair* pairs, PxU32 nbPairs)
{
    // Verificar si nuestro personaje está involucrado en esta colisión
    if (particles_list.empty()) return;

    auto* playerRB = static_cast<DynamicRigidBody*>(particles_list.front().get());
    PxRigidDynamic* ourPlayer = static_cast<PxRigidDynamic*>(playerRB->getActor());

    if (!ourPlayer) return;

    // Identificar cuál actor es nuestro personaje
    PxRigidDynamic* playerActor = nullptr;
    PxRigidActor* otherActor = nullptr;

    if (pairHeader.actors[0] == ourPlayer) {
        playerActor = static_cast<PxRigidDynamic*>(pairHeader.actors[0]);
        otherActor = pairHeader.actors[1];
    }
    else if (pairHeader.actors[1] == ourPlayer) {
        playerActor = static_cast<PxRigidDynamic*>(pairHeader.actors[1]);
        otherActor = pairHeader.actors[0];
    }
    else {
        return; // No es nuestro personaje
    }

    // Solo lectura - obtener posición actual
    PxTransform currentPose = playerActor->getGlobalPose();  // ✅ Permitido: solo lectura
    PxVec3 currentVelocity = playerActor->getLinearVelocity();  // ✅ Permitido: solo lectura

    // Procesar cada par de contacto
    for (PxU32 i = 0; i < nbPairs; i++) {
        const PxContactPair& contactPair = pairs[i];

        // Contacto comienza o persiste
        if (contactPair.events & (PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_PERSISTS)) {

            // Extraer puntos de contacto
            PxContactPairPoint contactPoints[32];
            PxU32 numContacts = contactPair.extractContacts(contactPoints, 32);

            for (PxU32 j = 0; j < numContacts; j++) {
                const PxContactPairPoint& point = contactPoints[j];

                // Solo verificar si es contacto con suelo
                if (point.normal.y > 0.7f && currentVelocity.y <= 0.5f) {
                    // Registrar contacto (solo almacenar puntero - permitido)
                    groundContacts.insert(otherActor);

                    // Verificar si necesita corrección (solo cálculo, sin modificar)
                    if (point.separation < -0.05f) {
                        needsGroundCorrection = true;
                        groundCorrectionY = currentPose.p.y - point.separation;

                        if (currentVelocity.y < 0) {
                            needsVelocityResetY = true;
                        }
                    }
                }
            }
        }

        // Contacto termina
        if (contactPair.events & PxPairFlag::eNOTIFY_TOUCH_LOST) {
            groundContacts.erase(otherActor);
        }
    }
}

void CharacterRBSystem::checkGroundFromContacts()
{
    wasGroundedLastFrame = isGrounded;
    isGrounded = !groundContacts.empty();
}

void CharacterRBSystem::processGroundCorrections()
{
    if (particles_list.empty()) return;

    auto* playerRB = static_cast<DynamicRigidBody*>(particles_list.front().get());
    PxRigidDynamic* actor = static_cast<PxRigidDynamic*>(playerRB->getActor());

    if (!actor) return;

    // Aplicar corrección de posición si es necesario
    if (needsGroundCorrection) {
        PxTransform pose = actor->getGlobalPose();
        pose.p.y = groundCorrectionY;
        actor->setGlobalPose(pose);  // ✅ Ahora sí permitido (fuera del callback)
        needsGroundCorrection = false;
    }

    // Aplicar reset de velocidad vertical si es necesario
    if (needsVelocityResetY) {
        PxVec3 vel = actor->getLinearVelocity();
        vel.y = 0.0f;
        actor->setLinearVelocity(vel);  // ✅ Ahora sí permitido (fuera del callback)
        needsVelocityResetY = false;
    }
}

// Métodos de control
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
    if (particles_list.empty() || !isGrounded) return;

    static float currentTime = 0.0f;
    if (lastJumpTime > 0 && (currentTime - lastJumpTime) < jumpCooldown) {
        return;
    }

    auto* playerRB = static_cast<DynamicRigidBody*>(particles_list.front().get());
    PxRigidDynamic* actor = static_cast<PxRigidDynamic*>(playerRB->getActor());

    if (!actor) return;

    float finalJumpForce = jumpForce + currentJumpCharge;

    // Limpiar contactos de suelo al saltar
    groundContacts.clear();

    // Aplicar impulso de salto
    actor->addForce(PxVec3(0.0f, finalJumpForce, 0.0f), PxForceMode::eIMPULSE);

    currentJumpCharge = 0.0f;
    isChargingJump = false;
    isGrounded = false;
    lastJumpTime = currentTime;

    std::cout << "SALTO! Fuerza: " << finalJumpForce << " N" << std::endl;
}

void CharacterRBSystem::update(float dt)
{
    static float currentTime = 0.0f;
    currentTime += dt;

    if (particles_list.empty()) return;

    // Actualizar carga de salto
    updateJumpCharge(dt);

    // Verificar suelo desde contactos
    checkGroundFromContacts();

    // Procesar correcciones de suelo (FUERA del callback)
    processGroundCorrections();

    // Aplicar movimiento
    applyMovement(dt);
    applyLateralDamping(dt);
    applyBounds();
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

    // Obtener velocidad actual y masa
    PxVec3 currentVelocity = actor->getLinearVelocity();
    float mass = actor->getMass();

    // Si hay input de movimiento
    if (moveDirection != 0) {
        // Fuerza objetivo en Newtons
        float targetForce = moveDirection * moveForce;

        // Suavizado de la fuerza aplicada
        float acceleration = 15.0f;
        float forceDelta = targetForce - currentMoveForce;
        currentMoveForce += forceDelta * std::min<float>(acceleration * dt, 1.0f);

        // Aplicar fuerza horizontal
        PxVec3 movementForce(currentMoveForce, 0.0f, 0.0f);
        actor->addForce(movementForce, PxForceMode::eFORCE);

        // Limitar velocidad máxima horizontal
        float maxHorizontalSpeed = 15.0f;
        if (std::abs(currentVelocity.x) > maxHorizontalSpeed) {
            currentVelocity.x = (currentVelocity.x > 0) ? maxHorizontalSpeed : -maxHorizontalSpeed;
            actor->setLinearVelocity(currentVelocity);
        }
    }
    else {
        // Cuando no hay input, reducir fuerza acumulada gradualmente
        float deceleration = 8.0f;
        currentMoveForce *= (1.0f - std::min<float>(deceleration * dt, 1.0f));

        // Resetear si es muy pequeña
        if (std::abs(currentMoveForce) < 0.5f) {
            currentMoveForce = 0.0f;
        }

        //// Frenado natural (solo en suelo)
        //if (isGrounded && std::abs(currentVelocity.x) > 0.1f) {
        //    float frictionForce = -currentVelocity.x * mass * 5.0f;
        //    actor->addForce(PxVec3(frictionForce, 0.0f, 0.0f), PxForceMode::eFORCE);

        //    // Detener completamente si la velocidad es muy baja
        //    if (std::abs(currentVelocity.x) < 0.2f) {
        //        currentVelocity.x = 0.0f;
        //        actor->setLinearVelocity(currentVelocity);
        //    }
        //}
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

    // Control aéreo reducido
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

    // Límite superior
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
    return rb->getPosition().y < -10.0f;
}