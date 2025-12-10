#pragma once
#include "Scene.h"
class Scene7 :
    public Scene
{
public:
    Scene7() = default;
    virtual ~Scene7() = default;
    void init() override;
    void handle_input(unsigned char key) override;
};

