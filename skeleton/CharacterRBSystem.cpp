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
    needsVelocityResetY(false), wasGroundedLastFrame(false), originalPos(pd.pos)
{
}

CharacterRBSystem::~CharacterRBSystem()
{
    std::cout << force_generators.size() << "\n";
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

// ============ MÉTODOS PARA MANEJAR CONTACTOS ESPECÍFICOS ============

void CharacterRBSystem::onFireContact()
{
    //std::cout << "CharacterRBSystem: ¡Me estoy quemando!" << std::endl;
    reset();
    SceneManager::instance().change_scene(INTRO);
}

void CharacterRBSystem::onGroundContact(float separation, const PxVec3& normal)
{
    // std::cout << "CharacterRBSystem: Contacto con suelo - separación: " 
    //           << separation << std::endl;

    // Solo procesar si es suelo (normal apunta hacia arriba)
    if (normal.y > 0.7f) {
        // Marcar como en suelo
        isGrounded = true;

        // Registrar para corrección posterior (fuera del callback)
        if (separation < -0.05f) {
            needsGroundCorrection = true;

            // Calcular la corrección
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
    // std::cout << "CharacterRBSystem: Perdí contacto con el suelo" << std::endl;
    isGrounded = false;
}

// ============ MÉTODOS DE CONTROL ============

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

    // Limpiar contacto con suelo al saltar
    isGrounded = false;
    wasGroundedLastFrame = true;

    // Aplicar impulso de salto
    actor->addForce(PxVec3(0.0f, finalJumpForce, 0.0f), PxForceMode::eIMPULSE);

    currentJumpCharge = 0.0f;
    isChargingJump = false;
    lastJumpTime = currentTime;

    std::cout << "SALTO! Fuerza: " << finalJumpForce << " N" << std::endl;
}

// ============ MÉTODOS DE ACTUALIZACIÓN ============

Particle* CharacterRBSystem::getCharacter()
{
    if (particles_list.empty()) return nullptr;
    else return particles_list.front().get(); 
}

void CharacterRBSystem::checkGroundFromContacts()
{
    wasGroundedLastFrame = isGrounded;
    // La variable isGrounded ahora se actualiza desde onGroundContact/onGroundLost
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
        actor->setGlobalPose(pose);
        needsGroundCorrection = false;

        // También resetear velocidad vertical si está cayendo
        PxVec3 vel = actor->getLinearVelocity();
        if (vel.y < 0) {
            vel.y = 0.0f;
            actor->setLinearVelocity(vel);
        }
    }
}

void CharacterRBSystem::update(float dt)
{
    static float currentTime = 0.0f;
    currentTime += dt;
    lastJumpTime = (lastJumpTime > 0) ? lastJumpTime - dt : 0.0f;

    if (particles_list.empty()) return;

    // Actualizar carga de salto
    updateJumpCharge(dt);

    // Procesar correcciones de suelo
    processGroundCorrections();

    // Aplicar movimiento
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

    // Resetear a posición original
    PxTransform originalTransform(originalPos);
    actor->setGlobalPose(originalTransform);

    // Resetear velocidad
    actor->setLinearVelocity(PxVec3(0, 0, 0));
    actor->setAngularVelocity(PxVec3(0, 0, 0));

    // Resetear variables de estado
    moveDirection = 0;
    currentMoveForce = 0.0f;
    currentJumpCharge = 0.0f;
    isChargingJump = false;
    isGrounded = false;
    wasGroundedLastFrame = false;

    // Limpiar correcciones
    needsGroundCorrection = false;
    needsVelocityResetY = false;
    groundCorrectionY = 0.0f;

    std::cout << "Jugador reseteado a posición original" << std::endl;
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

        // Frenado natural (solo en suelo)
        if (isGrounded && std::abs(currentVelocity.x) > 0.1f) {
            float frictionForce = -currentVelocity.x * mass * 5.0f;
            actor->addForce(PxVec3(frictionForce, 0.0f, 0.0f), PxForceMode::eFORCE);

            // Detener completamente si la velocidad es muy baja
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
    bool isOut = rb->getPosition().y < -10.0f;

    if (isOut) {
        // Nota: este método es const, no podemos resetear desde aquí
        // El reset debe manejarse desde otro lugar (Level::update por ejemplo)
        std::cout << "Jugador cayó fuera del mapa!" << std::endl;
    }

    return isOut;
}