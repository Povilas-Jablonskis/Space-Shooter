#include "PauseMenu.hpp"
#include "SceneStateMachine.hpp"
#include "Text.hpp"
#include "Renderer.hpp"
#include "SharedContext.hpp"
#include "InputManager.hpp"

#include <iostream>
#include <freeglut/freeglut.h>

PauseMenu::PauseMenu(SceneStateMachine& sceneStateMachine, SharedContext& context)
	: m_context(context), m_sceneStateMachine(sceneStateMachine)
{
}

void PauseMenu::onCreate()
{
	//Pause Menu
	const auto gotoMainMenuOption = std::make_shared<Text>("Go To Main Menu", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f),
	                                                       glm::vec2(48.0f, 60.0f), *m_context.font);
	gotoMainMenuOption->onMouseReleaseFunc = [=, this]
	{
		m_context.soundEngine->play2D("assets/Sounds/buttonselect/3.wav", GL_FALSE);

		m_sceneStateMachine.switchTo(ScenesEnum::MAIN);
	};
	m_texts.push_back(gotoMainMenuOption);

	const auto endGameOption = std::make_shared<Text>("End Game", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f),
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
	if (m_context.inputManager->isKeyActive(27))
	{
		m_context.soundEngine->play2D("assets/Sounds/buttonselect/5.wav", GL_FALSE);

		m_sceneStateMachine.switchTo(ScenesEnum::GAME_LIVE);
	}
}

void PauseMenu::draw(float)
{
	for (const auto& text : m_texts)
	{
		text->update(*m_context.inputManager);
	}

	m_context.renderer->draw(m_texts);
}
