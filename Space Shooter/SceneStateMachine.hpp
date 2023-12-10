#ifndef sceneStateMachine_hpp
#define sceneStateMachine_hpp

#include <memory>
#include <unordered_map>

#include "Scene.hpp"
#include "SceneName.hpp"

class SceneStateMachine
{
public:
	void processInput() const;
	void draw(float dt) const;

	void add(SceneName sceneName, const std::shared_ptr<Scene>& scene);
	void switchTo(const SceneName sceneId);
	void remove(const SceneName sceneId);

private:
	std::unordered_map<SceneName, std::shared_ptr<Scene>> m_scenes;
	std::shared_ptr<Scene> m_curScene{};
};

#endif
