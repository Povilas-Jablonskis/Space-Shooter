#ifndef soundsMenu_hpp
#define soundsMenu_hpp

#include "Scene.hpp"

class SoundsMenu : public Scene
{
public:
    SoundsMenu(std::shared_ptr<SceneStateMachine>, irrklang::ISoundEngine*, std::shared_ptr<InputManager>);

    void savePlayerConfig(irrklang::ISoundEngine*) const;

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

#endif