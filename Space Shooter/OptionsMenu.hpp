#ifndef optionsMenu_hpp
#define optionsMenu_hpp

#include "Scene.hpp"

class OptionsMenu : public Scene
{
public:
    OptionsMenu(std::shared_ptr<SceneStateMachine>, irrklang::ISoundEngine*, std::shared_ptr<InputManager>);

    void onCreate() override;
    void onDestroy() override;

    void onActivate() override;

    void processInput() override;

    void draw(const std::shared_ptr<Renderer>&, const float) override;
private:
    std::shared_ptr<InputManager> m_inputManager{ nullptr };
    irrklang::ISoundEngine* m_soundEngine{ nullptr };
    std::vector<std::shared_ptr<Text>> m_texts;
    std::shared_ptr<SceneStateMachine> m_sceneStateMachine{ nullptr };
};

#endif /* SceneSplashScreen_hpp */