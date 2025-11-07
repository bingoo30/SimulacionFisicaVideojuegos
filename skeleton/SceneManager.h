#pragma once
#include <vector>
#include "Scene.h"

enum States {
    INTRO,
    DEMO_SCENARIO,
    DEMO_CHARACTER,
    PARTICULAS_Y_PROYECTILES,
    GENERADORES_FUERZA,
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
    void set_initial_scene(int idx) { currScene = idx; };
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


