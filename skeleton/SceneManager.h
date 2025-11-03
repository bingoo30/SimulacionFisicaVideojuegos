#pragma once
#include <vector>
#include "Scene.h"

class SceneManager
{
public:
    //acceso global a la unica instancia
    static SceneManager& instance();

    //metodos públicos
    void change_scene(int idx);
    void add(Scene* s);
    Scene* getCurrScene();
    void set_initial_scene(int idx) { currScene = idx; };
    void clean();

private:
    //constructor y destructor privados
    SceneManager();
    ~SceneManager();

    //evitar copia o asignacion
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

private:
    std::vector<Scene*> scenes;
    int currScene = 0;
};


