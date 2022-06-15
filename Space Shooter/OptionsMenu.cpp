#include "OptionsMenu.hpp"
#include "SceneStateMachine.hpp"
#include "Text.hpp"
#include "Renderer.hpp"

OptionsMenu::OptionsMenu(std::shared_ptr<SceneStateMachine> sceneStateMachine, irrklang::ISoundEngine* soundEngine, std::shared_ptr<InputManager> inputManager)
    : m_sceneStateMachine(sceneStateMachine), m_soundEngine(soundEngine), m_inputManager(inputManager)
{

}

void OptionsMenu::onCreate()
{
	//Options
	auto controlsOption = std::make_shared<Text>("Controls", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 60.0f));
    controlsOption->onMouseReleaseFunc = [=]()
	{
		m_soundEngine->play2D("assets/Sounds/buttonselect/2.wav", GL_FALSE);

		m_sceneStateMachine->switchTo(ScenesEnum::CONTROLS);
	};
	m_texts.push_back(controlsOption);
	auto soundsOption = std::make_shared<Text>("Sounds", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 55.0f));
    soundsOption->onMouseReleaseFunc = [=]()
	{
		m_soundEngine->play2D("assets/Sounds/buttonselect/2.wav", GL_FALSE);

        m_sceneStateMachine->switchTo(ScenesEnum::SOUNDS);
	};
    m_texts.push_back(soundsOption);
	auto backOption = std::make_shared<Text>("Back", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 50.0f));
    backOption->onMouseReleaseFunc = [=]()
	{
		m_soundEngine->play2D("assets/Sounds/buttonselect/5.wav", GL_FALSE);

        m_sceneStateMachine->switchTo(ScenesEnum::MAIN);
	};
    m_texts.push_back(backOption);
}

void OptionsMenu::onDestroy() { }

void OptionsMenu::onActivate()
{
	m_inputManager->clearEverything();
}

void OptionsMenu::processInput()
{
	if (m_inputManager->getKey(27))
	{
		m_soundEngine->play2D("assets/Sounds/buttonselect/5.wav", GL_FALSE);

		m_sceneStateMachine->switchTo(ScenesEnum::MAIN);
	}
}

void OptionsMenu::draw(const std::shared_ptr<Renderer>& renderer, const float dt)
{
	for (auto& text : m_texts)
	{
		text->update(m_inputManager);
	}

	renderer->draw(m_texts);
}