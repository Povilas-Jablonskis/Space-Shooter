#include "GameWon.hpp"
#include "Text.hpp"
#include "Renderer.hpp"
#include "InputManager.hpp"

GameWon::GameWon(SceneStateMachine& sceneStateMachine, SharedContext& context)
	: m_context(context), m_sceneStateMachine(sceneStateMachine)
{
}

void GameWon::onCreate()
{
	//Game over
	const auto gameWonText = std::make_shared<Text>("You finished all waves! Congratulations!!!",
	                                                glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(45.0f, 50.0f),
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
	if (m_context.inputManager->isKeyActive(27))
	{
		m_sceneStateMachine.switchTo(ScenesEnum::MAIN);
	}
}

void GameWon::draw(float)
{
	for (const auto& text : m_texts)
	{
		text->update(*m_context.inputManager);
	}

	m_context.renderer->draw(m_texts);
}
