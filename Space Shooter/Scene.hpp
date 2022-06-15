#ifndef scene_hpp
#define scene_hpp

#include "Text.hpp"
#include "SceneStateMachine.hpp"
#include "InputManager.hpp"

#include <irrKlang/irrKlang.h>
#include <vector>
#include <memory>

class Renderer;

class Scene
{
public:
    virtual void onCreate() = 0; // Called when scene initially created. Called once per scene.
    virtual void onDestroy() = 0;

    virtual void onActivate() {}; // Called whenever a scene is activated.
    virtual void onDeactivate() {}; // Called whenever a scene is deactivated.

    virtual void processInput() {};
    virtual void draw(const std::shared_ptr<Renderer>&, const float) {};
};

#endif /* Scene_hpp */