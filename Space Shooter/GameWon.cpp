#include "GameWon.hpp"
#include "InputManager.hpp"
#include "Colors.hpp"

#include <Windows.h>

GameWon::GameWon(SceneStateMachine& sceneStateMachine, SharedContext& context)
	: m_context(context), m_sceneStateMachine(sceneStateMachine)
{
}

void GameWon::onCreate()
{
	//Game over
	const auto gameWonText = std::make_shared<Text>("You finished all waves! Congratulations!!!",
	                                                Colors::DEFAULT_TEXT, glm::vec2(45.0f, 50.0f),
	                                                *m_context.font);
	gameWonText->disable();
	m_texts.push_back(gameWonText);
}

void GameWon::onDestroy()
{
}

void GameWon::onActivate()
{
	m_context.inputManager->clearEverything();
}

void GameWon::processInput()
{
	if (m_context.inputManager->isKeyActive(VK_ESCAPE))
	{
		m_sceneStateMachine.switchTo(ScenesEnum::MAIN);
	}
}

void GameWon::draw(float)
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
