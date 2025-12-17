#include "SceneManager.h"
#include "Intro.h"
#include "SelectionLevel.h"
#include "Level.h"

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
    auto c = GetCamera();
    c->resetCamera();
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

void SceneManager::create_project()
{
    Scene* intro = new Intro();
    intro->init();
    add(intro);

    Scene* selection = new SelectionLevel();
    selection->init();
    add(selection);

    for (int i = 0; i < 1; ++i) {
        Scene* level_i = new Level(i+1);
        level_i->init();
        add(level_i);
    }

    set_initial_scene(INTRO);
}

void SceneManager::clean()
{
    scenes.clear();
}
