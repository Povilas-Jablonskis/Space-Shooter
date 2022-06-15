#include "MainMenu.hpp"
#include "SceneStateMachine.hpp"
#include "Text.hpp"
#include "Renderer.hpp"

MainMenu::MainMenu(std::shared_ptr<SceneStateMachine> sceneStateMachine, irrklang::ISoundEngine* soundEngine, std::shared_ptr<InputManager> inputManager)
    : m_sceneStateMachine(sceneStateMachine), m_soundEngine(soundEngine), m_inputManager(inputManager)
{

}

void MainMenu::onCreate()
{
    auto startGameOption = std::make_shared<Text>("Start Game", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 60.0f));
    startGameOption->onMouseReleaseFunc = [=]()
    {
        m_soundEngine->play2D("assets/Sounds/buttonselect/1.wav", GL_FALSE);

        m_sceneStateMachine->switchTo(ScenesEnum::PICK_YOUR_CHARACTER);
    };
    m_texts.push_back(startGameOption);

    auto optionsOption = std::make_shared<Text>("Options", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 55.0f));
    optionsOption->onMouseReleaseFunc = [=]()
    {
        m_soundEngine->play2D("assets/Sounds/buttonselect/1.wav", GL_FALSE);

        m_sceneStateMachine->switchTo(ScenesEnum::OPTIONS);
    };
    m_texts.push_back(optionsOption);

    auto endGameOption = std::make_shared<Text>("End Game", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 50.0f));
    endGameOption->onMouseReleaseFunc = [=]()
    {
        m_soundEngine->play2D("assets/Sounds/buttonselect/1.wav", GL_FALSE);

        #if _DEBUG
            std::cout << "exiting\n";
            (void)getchar();
        #endif
        exit(0);
    };
    m_texts.push_back(endGameOption);
}

void MainMenu::onActivate()
{
    m_inputManager->clearEverything();
}

void MainMenu::onDestroy() { }

void MainMenu::draw(const std::shared_ptr<Renderer>& renderer, const float dt)
{
    for (auto& text : m_texts)
    {
        text->update(m_inputManager);
    }

    renderer->draw(m_texts);
}