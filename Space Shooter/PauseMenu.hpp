#ifndef pauseMenu_hpp
#define pauseMenu_hpp

#include "Scene.hpp"

class PauseMenu : public Scene
{
public:
    PauseMenu(std::shared_ptr<SceneStateMachine>, irrklang::ISoundEngine*, std::shared_ptr<InputManager>);

    void onCreate() override;
    void onDestroy() override;

    void onActivate() override;

    void processInput() override;

    void draw(const std::shared_ptr<Renderer>&, const float) override;
private:
    std::shared_ptr<InputManager> m_inputManager{};
    irrklang::ISoundEngine* m_soundEngine{};
    std::vector<std::shared_ptr<Text>> m_texts;
    std::shared_ptr<SceneStateMachine> m_sceneStateMachine{};
};

#endif