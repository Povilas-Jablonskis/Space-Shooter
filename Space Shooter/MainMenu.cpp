#include "MainMenu.hpp"
#include "SceneStateMachine.hpp"
#include "Text.hpp"
#include "Renderer.hpp"
#include "SharedContext.hpp"
#include "InputManager.hpp"

#include <iostream>
#include <freeglut/freeglut.h>

MainMenu::MainMenu(SceneStateMachine& sceneStateMachine, SharedContext& context)
	: m_context(context), m_sceneStateMachine(sceneStateMachine)
{
}

void MainMenu::onCreate()
{
	const auto startGameOption = std::make_shared<Text>("Start Game", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f),
	                                                    glm::vec2(48.0f, 60.0f), *m_context.m_font);
	startGameOption->onMouseReleaseFunc = [=, this]
	{
		m_context.m_soundEngine->play2D("assets/Sounds/buttonselect/1.wav", GL_FALSE);

		m_sceneStateMachine.switchTo(PICK_YOUR_CHARACTER);
	};
	m_texts.push_back(startGameOption);

	const auto optionsOption = std::make_shared<Text>("Options", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f),
	                                                  glm::vec2(48.0f, 55.0f), *m_context.m_font);
	optionsOption->onMouseReleaseFunc = [=, this]
	{
		m_context.m_soundEngine->play2D("assets/Sounds/buttonselect/1.wav", GL_FALSE);

		m_sceneStateMachine.switchTo(OPTIONS);
	};
	m_texts.push_back(optionsOption);

	const auto endGameOption = std::make_shared<Text>("End Game", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f),
	                                                  glm::vec2(48.0f, 50.0f), *m_context.m_font);
	endGameOption->onMouseReleaseFunc = [=, this]
	{
		m_context.m_soundEngine->play2D("assets/Sounds/buttonselect/1.wav", GL_FALSE);

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
	m_context.m_inputManager->clearEverything();
}

void MainMenu::onDestroy()
{
}

void MainMenu::draw(float dt)
{
	for (const auto& text : m_texts)
	{
		text->update(*m_context.m_inputManager);
	}

	m_context.m_renderer->draw(m_texts);
}
