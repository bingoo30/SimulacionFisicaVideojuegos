#pragma once
#include "Scene.h"
class Scene0 :
    public Scene
{
public:
    Scene0() = default;
    void init() override;
    void handle_input(unsigned char key) override;
    void render_interface() override;
protected:
    std::string explication;
};

