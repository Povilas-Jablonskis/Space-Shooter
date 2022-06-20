#ifndef mainMenu_hpp
#define mainMenu_hpp

#include "Scene.hpp"

class MainMenu : public Scene
{
public:
    MainMenu(std::shared_ptr<SceneStateMachine>, irrklang::ISoundEngine*, std::shared_ptr<InputManager>);

    void onCreate() override;
    void onDestroy() override;

    void onActivate() override;

    void draw(const std::shared_ptr<Renderer>&, const float) override;
private:
    std::shared_ptr<InputManager> m_inputManager{ nullptr };
    irrklang::ISoundEngine* m_soundEngine{ nullptr };
    std::vector<std::shared_ptr<Text>> m_texts;
    std::shared_ptr<SceneStateMachine> m_sceneStateMachine{ nullptr };
};

#endif