#include "LevelManager.hpp"
#include "SpriteSheetManager.hpp"
#include "GameStateManager.hpp"
#include "Renderer.hpp"
#include "ConfigurationManager.hpp"
#include "CollisionManager.hpp"

#include <fstream>
#include <iostream>

namespace Engine
{
	LevelManager::LevelManager(const std::unique_ptr<SpriteSheetManager>& spriteSheetManager, irrklang::ISoundEngine* soundEngine, int t_characterSelectionIndex)
	{
		rapidxml::xml_document<> doc;
		rapidxml::xml_node<> * root_node;
		// Read the xml file into a vector
		std::ifstream theFile("Config/levels.xml");
		std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc.parse<0>(&buffer[0]);
		// Find our root node
		root_node = doc.first_node("Levels");
		// Iterate over the brewerys
		for (auto brewery_node = root_node->first_node("Level"); brewery_node; brewery_node = brewery_node->next_sibling("Level"))
		{
			m_levels.push(std::make_unique<Level>(brewery_node, spriteSheetManager, soundEngine, t_characterSelectionIndex));
		}

		theFile.close();
		doc.clear();

		m_accumulator = 0;
	}

	void LevelManager::renderCurrentLevel(float dt, const std::unique_ptr<GameStateManager>& gameStateManager, const std::unique_ptr<InputManager>& inputManager, const std::unique_ptr<CollisionManager>& collisionManager, const std::unique_ptr<Renderer>& renderer, const std::unique_ptr<ConfigurationManager>& configurationManager, const std::unique_ptr<SpriteSheetManager>& spriteSheetManager)
	{
		auto newTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME));
		auto frameTime = (newTime - m_currentTime) / 1000.0f;
		m_currentTime = newTime;

		m_accumulator += frameTime;

		while (m_accumulator >= dt)
		{
			if (!m_levels.empty())
			{
				bool updateStatus = getCurrentLevel()->update(dt, gameStateManager, inputManager, collisionManager);

				if (updateStatus && m_levels.size() > 1)
				{
					newLevel();
				}
			}

			m_accumulator -= dt;
		}

		if (!m_levels.empty())
		{
			getCurrentLevel()->render(dt, gameStateManager, inputManager, collisionManager, renderer, configurationManager, spriteSheetManager);
		}
	}
}