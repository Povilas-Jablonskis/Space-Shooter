#ifndef sceneStateMachine_hpp
#define sceneStateMachine_hpp

#include <memory>
#include <unordered_map>

#include "ScenesEnum.hpp"

class Renderer;
class Scene;

class SceneStateMachine
{
public:
	void processInput() const;
	void draw(float) const;

	void add(ScenesEnum, const std::shared_ptr<Scene>&);
	void switchTo(ScenesEnum);
	void remove(ScenesEnum);

private:
	std::unordered_map<ScenesEnum, std::shared_ptr<Scene>> m_scenes;
	std::shared_ptr<Scene> m_curScene{};
};

#endif
