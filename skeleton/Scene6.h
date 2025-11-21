#pragma once
#include "Scene.h"
class Scene6 :
    public Scene
{
public:
    Scene6() = default;
    void init() override;
    void handle_special_input(int key) override;
    void handle_input(unsigned char key) override;
};