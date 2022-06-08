#include "UIManager.hpp"
#include "Text.hpp"
#include "SpriteSheetManager.hpp"
#include "GameStateManager.hpp"
#include "Renderer.hpp"
#include "SpriteSheet.hpp"

namespace Engine
{
	void UIManager::renderUI(const std::shared_ptr<Renderer>& renderer)
	{
		renderer->draw(m_playerLives);

		renderer->draw(m_scoreBoard);

		for (auto& m_notification : m_notifications)
		{
			renderer->draw(m_notification);
		}
	}

	void UIManager::updateUI(const int score, const std::string& icon, const int lives, const float dt, const std::shared_ptr<InputManager>& inputManager, const std::shared_ptr<SpriteSheetManager>& spriteSheetManager)
	{
		m_scoreBoard.clear();

		auto scoreString = std::to_string(score);

		for (auto it = scoreString.begin(); it != scoreString.end(); ++it)
		{
			const auto i = std::distance(scoreString.begin(), it);
			std::string scoreStringNumber(1, scoreString[i]);
			auto option = std::make_shared<UIElementBase>(glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(70.0f + 4.0f * (i + 1.0f), 91.0f));
			option->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite("numeral" + scoreStringNumber + ".png"));
			m_scoreBoard.push_back(option);
		}

		m_playerLives.clear();

		auto option = std::make_shared<UIElementBase>(glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(6.0f, 91.0f));
		option->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(icon));
		m_playerLives.push_back(option);

		option = std::make_shared<UIElementBase>(glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(12.0f, 91.0f));
		option->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite("numeralX.png"));
		m_playerLives.push_back(option);

		auto livesString = std::to_string(lives);

		for (auto it = livesString.begin(); it != livesString.end(); ++it)
		{
			const auto i = std::distance(livesString.begin(), it);
			std::string livesStringNumber(1, livesString[i]);
			auto number = std::make_shared<UIElementBase>(glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(12.0f + 4.0f * (i + 1.0f), 91.0f));
			number->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite("numeral" + livesStringNumber + ".png"));
			m_playerLives.push_back(number);
		}

		for (auto& m_playerLive : m_playerLives)
		{
			m_playerLive->update(dt, inputManager);
		}

		for (auto& m_playerScoreElement : m_scoreBoard)
		{
			m_playerScoreElement->update(dt, inputManager);
		}

		for (auto& m_notification : m_notifications)
		{
			m_notification->update(inputManager);
		}
	}
}