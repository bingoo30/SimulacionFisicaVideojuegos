#include "CharacterRBSystem.h"
#include "UniformGeneratorRB.h"
#include "SceneManager.h"
#include <iostream>

using namespace physx;

extern PxPhysics* gPhysics;
extern PxScene* gScene;


CharacterRBSystem::CharacterRBSystem(const Player_Data& pd, const PxVec3& material, const PxFilterData& filter)
    : RigidBodySystem(pd, Particle_Deviation_Data(), 1, PxGeometryType::eBOX, false, material, filter),
    moveDirection(0.0f), moveSpeed(10.0f), jumpForce(8.0f), isGrounded(false)
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

            // Configuración física mejorada
            dyn->setMass(1.0f);
            dyn->setLinearDamping(0.8f);  // Para frenado más rápido
            dyn->setAngularDamping(10.0f);

            // Bloquear rotaciones y movimiento en Z
            dyn->setRigidDynamicLockFlags(
                PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
                PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y |
                PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z |
                PxRigidDynamicLockFlag::eLOCK_LINEAR_Z    // Solo movimiento en X
            );

            particles_list.push_back(std::unique_ptr<Particle>(new_p));
        }
        new_particles.clear();
    }
}

// Métodos de control
void CharacterRBSystem::moveLeft()
{
    moveDirection = -1;
}

void CharacterRBSystem::moveRight()
{
    moveDirection = 1;
}

void CharacterRBSystem::jump()
{
    if (particles_list.empty() || !isGrounded) return;

    auto* playerRB = static_cast<DynamicRigidBody*>(particles_list.front().get());
    PxRigidDynamic* actor = static_cast<PxRigidDynamic*>(playerRB->getActor());

    if (!actor) return;

    // Aplicar impulso vertical
    PxVec3 velocity = actor->getLinearVelocity();
    velocity.y = jumpForce;
    actor->setLinearVelocity(velocity);

    isGrounded = false;
    std::cout << "SALTO!" << std::endl;
}

void CharacterRBSystem::update(float dt)
{
    if (particles_list.empty()) return;

    checkGround();
    applyMovement(dt);
    applyBounds();
}

void CharacterRBSystem::checkGround()
{
    if (particles_list.empty()) return;

    auto* playerRB = static_cast<DynamicRigidBody*>(particles_list.front().get());
    PxRigidDynamic* actor = static_cast<PxRigidDynamic*>(playerRB->getActor());

    if (!actor) return;

    // Raycast hacia abajo para detectar suelo
    PxVec3 position = actor->getGlobalPose().p;
    PxReal rayLength = 0.6f;  // Un poco más de la mitad de la altura

    PxRaycastBuffer hit;
    PxVec3 rayStart = position + PxVec3(0, 0.1f, 0);  // Pequeño offset
    PxVec3 rayDir(0.0f, -1.0f, 0.0f);

    PxQueryFilterData filterData;
    filterData.flags |= PxQueryFlag::eSTATIC;  // Solo detectar estáticos

    bool hasHit = gScene->raycast(rayStart, rayDir, rayLength, hit,
        PxHitFlag::eDEFAULT, filterData);

    PxVec3 velocity = actor->getLinearVelocity();

    if (hasHit && velocity.y <= 0.1f) {
        isGrounded = true;

        // Ajustar posición para evitar hundimiento
        if (hit.block.distance < 0.55f) {
            PxTransform pose = actor->getGlobalPose();
            pose.p.y += (0.55f - hit.block.distance);
            actor->setGlobalPose(pose);
        }
    }
    else {
        isGrounded = false;
    }
}

void CharacterRBSystem::applyMovement(float dt)
{
    if (particles_list.empty()) return;

    auto* playerRB = static_cast<DynamicRigidBody*>(particles_list.front().get());
    PxRigidDynamic* actor = static_cast<PxRigidDynamic*>(playerRB->getActor());

    if (!actor) return;

    // Obtener velocidad actual
    PxVec3 currentVelocity = actor->getLinearVelocity();

    // Calcular velocidad objetivo en X
    float targetVelocityX = moveDirection * moveSpeed;

    // Suavizar el movimiento
    float acceleration = 20.0f;
    float newVelocityX;

    if (moveDirection != 0.0f) {
        // Aceleración cuando hay input
        newVelocityX = currentVelocity.x + (targetVelocityX - currentVelocity.x) *
            PxMin(acceleration * dt, 1.0f);
    }
    else {
        // Frenado más rápido cuando no hay input
        float brakeAcceleration = 30.0f;
        newVelocityX = currentVelocity.x * (1.0f - PxMin(brakeAcceleration * dt, 1.0f));
        if (fabs(newVelocityX) < 0.1f) newVelocityX = 0.0f;
    }

    // Limitar velocidad máxima
    float maxSpeed = 15.0f;
    if (fabs(newVelocityX) > maxSpeed) {
        newVelocityX = (newVelocityX > 0) ? maxSpeed : -maxSpeed;
    }

    // Aplicar nueva velocidad
    PxVec3 newVelocity(newVelocityX, currentVelocity.y, 0.0f);
    actor->setLinearVelocity(newVelocity);
}

void CharacterRBSystem::applyBounds()
{
    if (particles_list.empty()) return;

    auto* playerRB = static_cast<DynamicRigidBody*>(particles_list.front().get());
    PxRigidDynamic* actor = static_cast<PxRigidDynamic*>(playerRB->getActor());

    if (!actor) return;

    PxTransform pose = actor->getGlobalPose();

    // Límites en X (ajusta según tu nivel)
    float minX = -50.0f;
    float maxX = 50.0f;

    if (pose.p.x < minX) {
        pose.p.x = minX;
        actor->setGlobalPose(pose);

        PxVec3 vel = actor->getLinearVelocity();
        vel.x = 0;
        actor->setLinearVelocity(vel);
    }
    else if (pose.p.x > maxX) {
        pose.p.x = maxX;
        actor->setGlobalPose(pose);

        PxVec3 vel = actor->getLinearVelocity();
        vel.x = 0;
        actor->setLinearVelocity(vel);
    }

    // Respawn si cae
    if (pose.p.y < -20.0f) {
        pose.p = PxVec3(0, 5, 0);
        actor->setGlobalPose(pose);
        actor->setLinearVelocity(PxVec3(0));
        std::cout << "RESPAWN!" << std::endl;
    }
}

bool CharacterRBSystem::check_out_of_limit(Particle* p) const
{
    auto rb = static_cast<DynamicRigidBody*>(p);
    return rb->getPosition().y < -10;
}