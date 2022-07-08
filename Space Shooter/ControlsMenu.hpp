#ifndef controlsMenu_hpp
#define controlsMenu_hpp

#include "Scene.hpp"
#include "SceneStateMachine.hpp"
#include "SharedContext.hpp"

class Text;
class KeyBinding;

class ControlsMenu : public Scene
{
public:
    ControlsMenu(SceneStateMachine&, SharedContext&);

    void onCreate() override;
    void onDestroy() override;

    void onActivate() override;

    void processInput() override;

    void draw(float) override;

    void savePlayerConfig() const;
    std::string virtualKeyCodeToString(int);
private:
    std::vector<std::shared_ptr<Text>> m_texts;
    SharedContext& m_context;
    SceneStateMachine& m_sceneStateMachine;
};

#endif