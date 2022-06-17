#ifndef gameOver_hpp
#define gameOver_hpp

#include "Scene.hpp"

class GameOver : public Scene
{
public:
    GameOver(std::shared_ptr<SceneStateMachine>, std::shared_ptr<InputManager>);

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