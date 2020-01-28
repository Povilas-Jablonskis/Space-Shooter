#include "UIManager.hpp"
#include "Text.hpp"
#include "SpriteSheetManager.hpp"
#include "GameStateManager.hpp"
#include "Renderer.hpp"
#include "ConfigurationManager.hpp"
#include "SpriteSheet.hpp"

namespace Engine
{
	void UIManager::render(const float dt, const std::shared_ptr<GameStateManager>& gameStateManager, const std::shared_ptr<InputManager>& inputManager, const std::shared_ptr<Renderer>& renderer, const std::shared_ptr<ConfigurationManager>& configurationManager)
	{
		//Render & Update UI elements
		for (auto& m_playerLive : m_playerLives)
		{
			m_playerLive->update(dt, inputManager);
		}

		renderer->draw(m_playerLives);

		for (auto& score : m_scoreBoard)
		{
			score->update(dt, inputManager);
		}

		renderer->draw(m_scoreBoard);

		for (auto& m_notification : m_notifications)
		{
			m_notification.second->update(configurationManager, inputManager);
		}

		for (auto& m_notification : m_notifications)
		{
			renderer->draw(m_notification.second);
		}
	}

	void UIManager::updatePlayerLives(const std::shared_ptr<SpriteSheetManager>& spriteSheetManager, const std::string& icon, const int lives)
	{
		m_playerLives.clear();

		auto option = std::make_shared<UIElementBase>(glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(6.0f, 91.0f));
		option->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(icon));
		m_playerLives.push_back(option);

		option = std::make_shared<UIElementBase>(glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(12.0f, 91.0f));
		option->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite("numeralX.png"));
		m_playerLives.push_back(option);

		auto str = std::to_string(lives);

		for (auto it = str.begin(); it != str.end(); ++it)
		{
			const auto i = std::distance(str.begin(), it);
			std::string str2(1, str[i]);
			auto number = std::make_shared<UIElementBase>(glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(12.0f + 4.0f * (i + 1.0f), 91.0f));
			number->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite("numeral" + str2 + ".png"));
			m_playerLives.push_back(number);
		}
	}

	void UIManager::updatePlayerScore(const std::shared_ptr<SpriteSheetManager>& spriteSheetManager, const int score)
	{
		m_scoreBoard.clear();

		auto str = std::to_string(score);

		for (auto it = str.begin(); it != str.end(); ++it)
		{
			const auto i = std::distance(str.begin(), it);
			std::string str2(1, str[i]);
			auto option = std::make_shared<UIElementBase>(glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(70.0f + 4.0f * (i + 1.0f), 91.0f));
			option->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite("numeral" + str2 + ".png"));
			m_scoreBoard.push_back(option);
		}
	}
}