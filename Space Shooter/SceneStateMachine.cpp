#include "SceneStateMachine.hpp"
#include "Scene.hpp"

void SceneStateMachine::processInput() const
{
	if (curScene)
	{
		curScene->processInput();
	}
}

void SceneStateMachine::draw(const float dt) const
{
	if (curScene)
	{
		curScene->draw(dt);
	}
}

void SceneStateMachine::add(ScenesEnum sceneName, const std::shared_ptr<Scene>& scene)
{
	const auto inserted = scenes.insert(std::make_pair(sceneName, scene));

	inserted.first->second->onCreate();
}

void SceneStateMachine::remove(const ScenesEnum id)
{
	const auto it = scenes.find(id);
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

void SceneStateMachine::switchTo(const ScenesEnum id)
{
	const auto it = scenes.find(id);
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
