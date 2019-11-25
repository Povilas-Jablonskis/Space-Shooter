#include "UIManager.hpp"
#include "Text.hpp"
#include "SpriteSheetManager.hpp"
#include "GameStateManager.hpp"
#include "Renderer.hpp"
#include "ConfigurationManager.hpp"
#include "SpriteSheet.hpp"

namespace Engine
{
	void UIManager::render(float dt, const std::unique_ptr<GameStateManager>& gameStateManager, const std::unique_ptr<InputManager>& inputManager, const std::unique_ptr<Renderer>& renderer, const std::unique_ptr<ConfigurationManager>& configurationManager)
	{
		//Render & Update UI elements
		for (auto it = m_playerLives.begin(); it != m_playerLives.end(); ++it)
		{
			(*it)->update(dt, inputManager);
		}

		renderer->draw(m_playerLives);

		for (auto it = m_scoreBoard.begin(); it != m_scoreBoard.end(); ++it)
		{
			(*it)->update(dt, inputManager);
		}

		renderer->draw(m_scoreBoard);

		for (auto it = m_notifications.begin(); it != m_notifications.end(); ++it)
		{
			(*it).second->update(dt, configurationManager, inputManager);
		}

		for (auto it = m_notifications.begin(); it != m_notifications.end(); ++it)
		{
			renderer->draw((*it).second);
		}
	}

	void UIManager::updatePlayerLives(const std::unique_ptr<SpriteSheetManager>& spriteSheetManager, const std::string& icon, int lives)
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
			auto i = std::distance(str.begin(), it);
			std::string str2(1, str[i]);
			auto option = std::make_shared<UIElementBase>(glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(12.0f + (4 * (i + 1)), 91.0f));
			option->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite("numeral" + str2 + ".png"));
			m_playerLives.push_back(option);
		}
	}

	void UIManager::updatePlayerScore(const std::unique_ptr<SpriteSheetManager>& spriteSheetManager, int score)
	{
		m_scoreBoard.clear();

		auto str = std::to_string(score);

		for (auto it = str.begin(); it != str.end(); ++it)
		{
			auto i = std::distance(str.begin(), it);
			std::string str2(1, str[i]);
			auto option = std::make_shared<UIElementBase>(glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(70.0f + (4 * (i + 1)), 91.0f));
			option->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite("numeral" + str2 + ".png"));
			m_scoreBoard.push_back(option);
		}
	}
}