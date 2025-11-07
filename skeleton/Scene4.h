#pragma once
#include "Scene.h"
#include "WhirlwindForceGenerator.h"
#include "ExplosionForceGenerator.h"
#include "WindForceGenerator.h"
class Scene4 :
    public Scene
{
public:
    Scene4()= default;
    void init() override;
    void handle_input(unsigned char key) override;
    void render_interface()override;
protected:
    WhirlwindForceGenerator* whirlwind;
    ExplosionForceGenerator* explosion;
    WindForceGenerator* wind;
    std::string explication;
};