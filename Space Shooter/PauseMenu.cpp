#include "PauseMenu.hpp"
#include "InputManager.hpp"

#include <iostream>
#include <freeglut/freeglut.h>

#include "Colors.hpp"
#include "Sounds.hpp"

PauseMenu::PauseMenu(SceneStateMachine& sceneStateMachine, SharedContext& context)
	: m_context(context), m_sceneStateMachine(sceneStateMachine)
{
}

void PauseMenu::onCreate()
{
	//Pause Menu
	const auto gotoMainMenuOption = std::make_shared<Text>("Go To Main Menu", Colors::DEFAULT_TEXT,
	                                                       glm::vec2(48.0f, 60.0f), *m_context.font);
	gotoMainMenuOption->onMouseReleaseFunc = [=, this]
	{
		m_context.soundEngine->play2D(Sounds::MENU_SELECT_GO_NEXT_OPTION, GL_FALSE);

		m_sceneStateMachine.switchTo(SceneName::MAIN);
	};
	m_texts.push_back(gotoMainMenuOption);

	const auto endGameOption = std::make_shared<Text>("End Game", Colors::DEFAULT_TEXT,
	                                                  glm::vec2(48.0f, 55.0f), *m_context.font);
	endGameOption->onMouseReleaseFunc = []
	{
#if _DEBUG
		std::cout << "exiting\n";
		(void)getchar();
#endif
		glutLeaveMainLoop();
	};
	m_texts.push_back(endGameOption);
}

void PauseMenu::onDestroy()
{
}

void PauseMenu::onActivate()
{
	m_context.inputManager->clearEverything();
}

void PauseMenu::processInput()
{
	if (m_context.inputManager->isKeyActive(VK_ESCAPE))
	{
		m_context.soundEngine->play2D(Sounds::MENU_SELECT_GO_BACK_OPTION, GL_FALSE);

		m_sceneStateMachine.switchTo(SceneName::GAME_LIVE);
	}
}

void PauseMenu::draw(float)
{
	for (const auto& text : m_texts)
	{
		text->update();
	}

	for (const auto& text : m_texts)
	{
		m_context.inputManager->checkInteraction(text);
	}

	m_context.renderer->draw(m_texts);
}
