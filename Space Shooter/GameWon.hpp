#ifndef gameWon_hpp
#define gameWon_hpp

#include "Scene.hpp"

class GameWon : public Scene
{
public:
    GameWon(std::shared_ptr<SceneStateMachine>, std::shared_ptr<InputManager>);

    void onCreate() override;
    void onDestroy() override;

    void onActivate() override;

    void processInput() override;

    void draw(const std::shared_ptr<Renderer>&, const float) override;
private:
    std::shared_ptr<InputManager> m_inputManager{ nullptr };
    std::vector<std::shared_ptr<Text>> m_texts;
    std::shared_ptr<SceneStateMachine> m_sceneStateMachine{ nullptr };
};

#endif /* SceneSplashScreen_hpp */