#include "MainMenu.hpp"
#include "InputManager.hpp"

#include <iostream>
#include <freeglut/freeglut.h>

#include "Colors.hpp"
#include "Sounds.hpp"

MainMenu::MainMenu(SceneStateMachine& sceneStateMachine, SharedContext& context)
	: m_context(context), m_sceneStateMachine(sceneStateMachine)
{
}

void MainMenu::onCreate()
{
	const auto startGameOption = std::make_shared<Text>("Start Game", Colors::DEFAULT_TEXT,
	                                                    glm::vec2(48.0f, 60.0f), *m_context.font);
	startGameOption->onMouseClickFunc = [=, this]
	{
		m_context.soundEngine->play2D(Sounds::MENU_SELECT_GO_NEXT_OPTION, GL_FALSE);

		m_sceneStateMachine.switchTo(SceneName::PICK_YOUR_CHARACTER);
	};
	m_texts.push_back(startGameOption);

	const auto optionsOption = std::make_shared<Text>("Options", Colors::DEFAULT_TEXT,
	                                                  glm::vec2(48.0f, 55.0f), *m_context.font);
	optionsOption->onMouseReleaseFunc = [=, this]
	{
		m_context.soundEngine->play2D(Sounds::MENU_SELECT_GO_NEXT_OPTION, GL_FALSE);

		m_sceneStateMachine.switchTo(SceneName::OPTIONS);
	};
	m_texts.push_back(optionsOption);

	const auto endGameOption = std::make_shared<Text>("End Game", Colors::DEFAULT_TEXT,
	                                                  glm::vec2(48.0f, 50.0f), *m_context.font);
	endGameOption->onMouseReleaseFunc = [=, this]
	{
		m_context.soundEngine->play2D(Sounds::MENU_SELECT_GO_NEXT_OPTION, GL_FALSE);

#if _DEBUG
		std::cout << "exiting\n";
		(void)getchar();
#endif
		glutLeaveMainLoop();
	};
	m_texts.push_back(endGameOption);
}

void MainMenu::onActivate()
{
	m_context.inputManager->clearEverything();
}

void MainMenu::onDestroy()
{
}

void MainMenu::draw(float)
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
