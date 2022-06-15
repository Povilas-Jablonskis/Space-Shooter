#include "GameWon.hpp"
#include "SceneStateMachine.hpp"
#include "Text.hpp"
#include "Renderer.hpp"

GameWon::GameWon(std::shared_ptr<SceneStateMachine> sceneStateMachine, std::shared_ptr<InputManager> inputManager)
	: m_sceneStateMachine(sceneStateMachine), m_inputManager(inputManager)
{

}

void GameWon::onCreate()
{
	//Game over
	auto gameWonText = std::make_shared<Text>("You finished all waves! Congratulations!!!", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(45.0f, 50.0f));
	gameWonText->disable();
	m_texts.push_back(gameWonText);
}

void GameWon::onDestroy() { }

void GameWon::onActivate()
{
	m_inputManager->clearEverything();
}

void GameWon::processInput()
{
	if (m_inputManager->getKey(27))
	{
		m_sceneStateMachine->switchTo(ScenesEnum::MAIN);
	}
}

void GameWon::draw(const std::shared_ptr<Renderer>& renderer, const float dt)
{
	for (auto& text : m_texts)
	{
		text->update(m_inputManager);
	}

	renderer->draw(m_texts);
}