#include "Application.hpp"
#include "Renderer.hpp"
#include "SpriteSheetManager.hpp"
#include "MenuManager.hpp"
#include "LevelManager.hpp"
#include "InputManager.hpp"
#include "GameStateManager.hpp"
#include "CollisionManager.hpp"
#include "ConfigurationManager.hpp"
#include "Shader.hpp"

#include <fstream>
#include "rapidxml/rapidxml_print.hpp"

namespace Engine
{
	Application::Application()
	{
		getRenderer()->addShader("shader", std::make_shared<Shader>("shader.vert", "shader.frag"));
		getRenderer()->addShader("textshader", std::make_shared<Shader>("textshader.vert", "textshader.frag"));

		loadPlayerConfig();
		getSpritesheetManager()->loadSpriteSheetsFromConfig();
		getMenuManager()->loadPlayerModels(getSpritesheetManager());
		getMenuManager()->initGameMenus(getSoundEngine(), getInputManager().get(), getGameStateManager(), getSpritesheetManager());
	}

	void Application::loadPlayerConfig() const
	{
		{
			auto doc = new rapidxml::xml_document<>();
			// Read the xml file into a vector
			std::ifstream theFile("Config/keyBindingSettings.xml");
			std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
			buffer.push_back('\0');
			// Parse the buffer using the xml file parsing library into doc 
			doc->parse<0>(&buffer[0]);

			for (auto brewery_node = doc->first_node("KeyBindings"); brewery_node; brewery_node = brewery_node->next_sibling("KeyBindings"))
			{
				for (auto beer_node = brewery_node->first_node("KeyBinding"); beer_node; beer_node = beer_node->next_sibling("KeyBinding"))
				{
					getInputManager()->setKeyBinding(keyBinding(beer_node->first_attribute("key")->value(), std::stoi(beer_node->first_attribute("value")->value())));
				}
			}

			theFile.close();
			doc->clear();
			delete doc;
		}

		{
			auto doc = new rapidxml::xml_document<>();
			// Read the xml file into a vector
			std::ifstream theFile("Config/soundSettings.xml");
			std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
			buffer.push_back('\0');
			// Parse the buffer using the xml file parsing library into doc 
			doc->parse<0>(&buffer[0]);

			for (auto brewery_node = doc->first_node("Volume"); brewery_node; brewery_node = brewery_node->next_sibling("Volume"))
			{
				getSoundEngine()->setSoundVolume(std::stof(brewery_node->first_attribute("value")->value()));
			}

			theFile.close();
			doc->clear();
			delete doc;
		}
	}

	void Application::render() const
	{
		glClear(GL_COLOR_BUFFER_BIT);

		const auto menus = getMenuManager()->getMenus();

		if (getMenuManager()->getLevelManager())
		{
			getMenuManager()->getLevelManager()->renderCurrentLevel(m_dt, getGameStateManager(), getInputManager(), getCollisionManager(), getRenderer(), getConfigurationManager(), getSpritesheetManager());
		}

		if (!menus->empty() && (getGameStateManager()->getGameState() == IN_MENU || getGameStateManager()->getGameState() == IN_PAUSED_MENU))
		{
			getMenuManager()->renderCurrentMenu(getRenderer(), m_dt, getConfigurationManager(), getInputManager());
		}

		glutSwapBuffers();
	}
}