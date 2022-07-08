#include "PauseMenu.hpp"
#include "SceneStateMachine.hpp"
#include "Text.hpp"
#include "Renderer.hpp"
#include "SharedContext.hpp"
#include "InputManager.hpp"

#include <iostream>

PauseMenu::PauseMenu(SceneStateMachine& sceneStateMachine, SharedContext& context)
	: m_sceneStateMachine(sceneStateMachine), m_context(context)
{

}

void PauseMenu::onCreate()
{
	//Pause Menu
	auto gotoMainMenuOption = std::make_shared<Text>("Go To Main Menu", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 60.0f), *m_context.m_font);
	gotoMainMenuOption->onMouseReleaseFunc = [=]()
	{
		m_context.m_soundEngine->play2D("assets/Sounds/buttonselect/3.wav", GL_FALSE);

		m_sceneStateMachine.switchTo(ScenesEnum::MAIN);
	};
	m_texts.push_back(gotoMainMenuOption);

	auto endGameOption = std::make_shared<Text>("End Game", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 55.0f), *m_context.m_font);
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
	m_context.m_inputManager->clearEverything();
}

void PauseMenu::processInput()
{
	if (m_context.m_inputManager->getKey(27))
	{
		m_context.m_soundEngine->play2D("assets/Sounds/buttonselect/5.wav", GL_FALSE);

		m_sceneStateMachine.switchTo(ScenesEnum::GAME_LIVE);
	}
}

void PauseMenu::draw(float dt)
{
	for (auto& text : m_texts)
	{
		text->update(*m_context.m_inputManager);
	}

	m_context.m_renderer->draw(m_texts);
}