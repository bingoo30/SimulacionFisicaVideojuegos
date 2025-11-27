#pragma once
#include "Scene.h"
class SpringForceGenerator;
class AnchoredSpringForceGenerator;
class Scene6 :
    public Scene
{
public:
    Scene6() = default;
    void init() override;
    void handle_special_input(int key) override;
    void handle_input(unsigned char key) override;
protected:
    SpringForceGenerator* muelleConParticula;
    AnchoredSpringForceGenerator* muelleSinParticula;
};