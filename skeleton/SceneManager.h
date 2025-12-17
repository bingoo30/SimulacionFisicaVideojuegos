#pragma once
#include <vector>
#include "Scene.h"

enum States {
    INTRO, 
    SELECTION, 
    LEVEL1,
    LEVEL2,
    VICTORY,
    DEFEAT,
    LAST_STATE,
};
class SceneManager
{
public:
    //acceso global a la unica instancia
    static SceneManager& instance();

    //metodos públicos
    void change_scene(int idx);
    void add(Scene* s);
    Scene* getCurrScene();
    void create_project_levels();
    void set_initial_scene(int idx) { 
        currScene = idx; 
        scenes[currScene]->enter();
    };
    void clean();

private:
#pragma region atributos 
    //escenas que tengo en el juego
    std::vector<Scene*> scenes;
    //escena actual
    int currScene = 0;
#pragma endregion

    //constructor y destructor privados
    SceneManager();
    ~SceneManager();

    //evitar copia o asignacion
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
};


