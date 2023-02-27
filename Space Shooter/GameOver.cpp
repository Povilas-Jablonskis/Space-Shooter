#include "GameOver.hpp"
#include "SceneStateMachine.hpp"
#include "Text.hpp"
#include "Renderer.hpp"
#include "SharedContext.hpp"
#include "InputManager.hpp"

GameOver::GameOver(SceneStateMachine& sceneStateMachine, SharedContext& context)
	: m_context(context), m_sceneStateMachine(sceneStateMachine)
{
}

void GameOver::onCreate()
{
	//Game over
	const auto gameOverText = std::make_shared<Text>("Game over!", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f),
	                                                 glm::vec2(45.0f, 50.0f), *m_context.m_font);
	gameOverText->disable();
	m_texts.push_back(gameOverText);
}

void GameOver::onDestroy()
{
}

void GameOver::onActivate()
{
	m_context.m_inputManager->clearEverything();
}

void GameOver::processInput()
{
	if (m_context.m_inputManager->getKey(27))
	{
		m_sceneStateMachine.switchTo(MAIN);
	}
}

void GameOver::draw(float dt)
{
	for (const auto& text : m_texts)
	{
		text->update(*m_context.m_inputManager);
	}

	m_context.m_renderer->draw(m_texts);
}
