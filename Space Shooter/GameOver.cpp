#include "GameOver.hpp"
#include "InputManager.hpp"

GameOver::GameOver(SceneStateMachine& sceneStateMachine, SharedContext& context)
	: m_context(context), m_sceneStateMachine(sceneStateMachine)
{
}

void GameOver::onCreate()
{
	//Game over
	const auto gameOverText = std::make_shared<Text>("Game over!", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f),
	                                                 glm::vec2(45.0f, 50.0f), *m_context.font);
	gameOverText->disable();
	m_texts.push_back(gameOverText);
}

void GameOver::onDestroy()
{
}

void GameOver::onActivate()
{
	m_context.inputManager->clearEverything();
}

void GameOver::processInput()
{
	if (m_context.inputManager->isKeyActive(27))
	{
		m_sceneStateMachine.switchTo(ScenesEnum::MAIN);
	}
}

void GameOver::draw(float)
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
