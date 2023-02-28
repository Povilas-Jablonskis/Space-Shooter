#include "SceneStateMachine.hpp"

void SceneStateMachine::processInput() const
{
	if (m_curScene)
	{
		m_curScene->processInput();
	}
}

void SceneStateMachine::draw(const float deltaTime) const
{
	if (m_curScene)
	{
		m_curScene->draw(deltaTime);
	}
}

void SceneStateMachine::add(ScenesEnum sceneName, const std::shared_ptr<Scene>& scene)
{
	const auto [iterator, success] = m_scenes.insert(std::make_pair(sceneName, scene));

	iterator->second->onCreate();
}

void SceneStateMachine::remove(const ScenesEnum sceneId)
{
	if (const auto sceneIterator = m_scenes.find(sceneId); sceneIterator != m_scenes.end())
	{
		if (m_curScene == sceneIterator->second)
		{
			m_curScene = nullptr;
		}

		sceneIterator->second->onDestroy();

		m_scenes.erase(sceneIterator);
	}
}

void SceneStateMachine::switchTo(const ScenesEnum sceneId)
{
	if (const auto sceneIterator = m_scenes.find(sceneId); sceneIterator != m_scenes.end())
	{
		if (m_curScene)
		{
			m_curScene->onDeactivate();
		}

		m_curScene = sceneIterator->second;

		m_curScene->onActivate();
	}
}
