#include "SceneManager.h"
#include "Intro.h"
#include "SelectionLevel.h"
#include "Level.h"
#include "GameOver.h"
#include "Victory.h"

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
    if (idx == -1)
        idx = currScene;
    if (idx < 0 || idx >= scenes.size())
        return;
    scenes[currScene]->exit();
    currScene = idx;
    scenes[currScene]->enter();
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

    for (int i = 0; i < 2; ++i) {
        Scene* level_i = new Level(i+1);
        level_i->init();
        add(level_i);
    }
    Scene* victory = new Victory();
    victory->init();
    add(victory);

    Scene* gameover = new GameOver();
    gameover->init();
    add(gameover);

    set_initial_scene(DEFEAT);
}

void SceneManager::clean()
{
    scenes.clear();
}
