#include "GameOver.hpp"
#include "SceneStateMachine.hpp"
#include "Text.hpp"
#include "Renderer.hpp"

GameOver::GameOver(std::shared_ptr<SceneStateMachine> sceneStateMachine, std::shared_ptr<InputManager> inputManager)
	: m_sceneStateMachine(sceneStateMachine), m_inputManager(inputManager)
{

}

void GameOver::onCreate()
{
	//Game over
	auto gameOverText = std::make_shared<Text>("Game over!", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(45.0f, 50.0f));
	gameOverText->disable();
	m_texts.push_back(gameOverText);
}

void GameOver::onDestroy() { }

void GameOver::onActivate()
{
	m_inputManager->clearEverything();
}

void GameOver::processInput()
{
	if (m_inputManager->getKey(27))
	{
		m_sceneStateMachine->switchTo(ScenesEnum::MAIN);
	}
}

void GameOver::draw(const std::shared_ptr<Renderer>& renderer, const float dt)
{
	for (auto& text : m_texts)
	{
		text->update(m_inputManager);
	}

	renderer->draw(m_texts);
}