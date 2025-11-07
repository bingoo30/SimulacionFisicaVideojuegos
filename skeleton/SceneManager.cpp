#include "SceneManager.h"

SceneManager::SceneManager() : scenes(), currScene(0) {}

SceneManager::~SceneManager()
{
    for (auto s : scenes)
        delete s;
    scenes.clear();
}

SceneManager& SceneManager::instance()
{
    static SceneManager inst; // Se crea la primera vez, y se destruye automáticamente al cerrar el programa
    return inst;
}

void SceneManager::change_scene(int idx)
{
    if (idx < 0 || idx >= scenes.size() || idx == currScene)
        return;

    scenes[currScene]->exit();
    currScene = idx;
    scenes[currScene]->enter();
    resetCamera();
}

void SceneManager::add(Scene* s)
{
    if (s != nullptr)
        scenes.push_back(s);
}

Scene* SceneManager::getCurrScene()
{
    if (scenes.empty()) return nullptr;
    return scenes[currScene];
}

void SceneManager::clean()
{
    scenes.clear();
}
