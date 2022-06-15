#include "PauseMenu.hpp"
#include "SceneStateMachine.hpp"
#include "Text.hpp"
#include "Renderer.hpp"

PauseMenu::PauseMenu(std::shared_ptr<SceneStateMachine> sceneStateMachine, irrklang::ISoundEngine* soundEngine, std::shared_ptr<InputManager> inputManager)
	: m_sceneStateMachine(sceneStateMachine), m_soundEngine(soundEngine), m_inputManager(inputManager)
{

}

void PauseMenu::onCreate()
{
	//Pause Menu
	auto gotoMainMenuOption = std::make_shared<Text>("Go To Main Menu", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 60.0f));
	gotoMainMenuOption->onMouseReleaseFunc = [=]()
	{
		m_soundEngine->play2D("assets/Sounds/buttonselect/3.wav", GL_FALSE);

		m_sceneStateMachine->switchTo(ScenesEnum::MAIN);
	};
	m_texts.push_back(gotoMainMenuOption);

	auto endGameOption = std::make_shared<Text>("End Game", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 55.0f));
	endGameOption->onMouseReleaseFunc = []()
	{
		#if _DEBUG
			std::cout << "exiting\n";
			(void)getchar();
		#endif
		exit(0);
	};
	m_texts.push_back(endGameOption);
}

void PauseMenu::onDestroy() { }

void PauseMenu::onActivate()
{
	m_inputManager->clearEverything();
}

void PauseMenu::processInput()
{
	if (m_inputManager->getKey(27))
	{
		m_soundEngine->play2D("assets/Sounds/buttonselect/5.wav", GL_FALSE);

		m_sceneStateMachine->switchTo(ScenesEnum::GAME_LIVE);
	}
}

void PauseMenu::draw(const std::shared_ptr<Renderer>& renderer, const float dt)
{
	for (auto& text : m_texts)
	{
		text->update(m_inputManager);
	}

	renderer->draw(m_texts);
}