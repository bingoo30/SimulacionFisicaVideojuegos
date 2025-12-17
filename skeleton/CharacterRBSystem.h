#pragma once
#include "RigidBodySystem.h"
#include "StructForEntities.h"

class CharacterRBSystem : public RigidBodySystem
{
private:
    int moveDirection;  // -1 = izquierda, 0 = quieto, 1 = derecha
    float moveSpeed;
    float jumpForce;
    bool isGrounded;

public:
    CharacterRBSystem(const Player_Data& pd, const physx::PxVec3& material, const physx::PxFilterData& filter);
    ~CharacterRBSystem() = default;

    void init() override;
    void spawn(bool withRender = true, bool isStatic = false) override;
    bool check_out_of_limit(Particle* p) const override;

    // Métodos de control
    void moveLeft();
    void moveRight();
    void jump();

    // Actualizar cada frame
    void update(float dt);

private:
    void checkGround();
    void applyMovement(float dt);
    void applyBounds();
};