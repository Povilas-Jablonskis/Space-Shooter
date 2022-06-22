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
    void processInput();
    void draw(const std::shared_ptr<Renderer>&, const float);

    void add(ScenesEnum, std::shared_ptr<Scene>);
    void switchTo(ScenesEnum);
    void remove(ScenesEnum);
private:
    std::unordered_map<ScenesEnum, std::shared_ptr<Scene>> scenes;
    std::shared_ptr<Scene> curScene{};
    unsigned int insertedSceneID{};
};

#endif