#include "SceneStateMachine.hpp"
#include "Scene.hpp"

void SceneStateMachine::processInput()
{
    if (curScene)
    {
        curScene->processInput();
    }
}

void SceneStateMachine::draw(float dt)
{
    if (curScene)
    {
        curScene->draw(dt);
    }
}

void SceneStateMachine::add(ScenesEnum sceneName, const std::shared_ptr<Scene>& scene)
{
    auto inserted = scenes.insert(std::make_pair(sceneName, scene));

    inserted.first->second->onCreate();
}

void SceneStateMachine::remove(ScenesEnum id)
{
    auto it = scenes.find(id);
    if (it != scenes.end())
    {
        if (curScene == it->second)
        {
            curScene = nullptr;
        }

        it->second->onDestroy();

        scenes.erase(it);
    }
}

void SceneStateMachine::switchTo(ScenesEnum id)
{
    auto it = scenes.find(id);
    if (it != scenes.end())
    {
        if (curScene)
        {
            curScene->onDeactivate();
        }

        curScene = it->second;

        curScene->onActivate();
    }
}