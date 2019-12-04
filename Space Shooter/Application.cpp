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

#include <ctime>
#include "rapidxml_print.hpp"

namespace Engine
{
	Application::~Application()
	{
		savePlayerConfig();
	}

	Application::Application()
	{
		srand(static_cast<int>(time(NULL)));

		getRenderer()->addShader("shader", std::make_shared<Shader>("shader.vert", "shader.frag"));
		getRenderer()->addShader("textshader", std::make_shared<Shader>("textshader.vert", "textshader.frag"));

		loadPlayerConfig();
		getSpritesheetManager()->loadSpriteSheetsFromConfig();
		getMenuManager()->loadPlayerModels(getSpritesheetManager());
		getMenuManager()->initGameMenus(getSoundEngine(), getInputManager().get(), getGameStateManager(), getSpritesheetManager());
	}

	void Application::savePlayerConfig()
	{
		const auto keyBindings = *getInputManager()->getKeyBindings();
		rapidxml::xml_document<> doc;

		rapidxml::xml_node<>* Config = doc.allocate_node(rapidxml::node_element, "Config");

		doc.append_node(Config);

		rapidxml::xml_node<>* KeyBindings = doc.allocate_node(rapidxml::node_element, "KeyBindings");

		for (auto keyBinding : keyBindings)
		{
			rapidxml::xml_node<>* KeyBinding = doc.allocate_node(rapidxml::node_element, "KeyBinding");
			auto attribute_value = doc.allocate_string(keyBinding.first.c_str());
			KeyBinding->append_attribute(doc.allocate_attribute("key", attribute_value));
			attribute_value = doc.allocate_string(std::to_string(keyBinding.second).c_str());
			KeyBinding->append_attribute(doc.allocate_attribute("value", attribute_value));
			KeyBindings->append_node(KeyBinding);
		}

		Config->append_node(KeyBindings);

		rapidxml::xml_node<>* Volume = doc.allocate_node(rapidxml::node_element, "Volume");
		auto attribute_value = doc.allocate_string(std::to_string(getSoundEngine()->getSoundVolume()).c_str());
		Volume->append_attribute(doc.allocate_attribute("value", attribute_value));
		Config->append_node(Volume);

		std::ofstream file_stored("Config/playerConfig.xml");
		file_stored << doc;
		file_stored.close();
		doc.clear();
	}

	void Application::loadPlayerConfig()
	{
		rapidxml::xml_document<> doc;
		rapidxml::xml_node<> * root_node;
		// Read the xml file into a vector
		std::ifstream theFile("Config/playerConfig.xml");
		std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc.parse<0>(&buffer[0]);
		// Find our root node
		root_node = doc.first_node("Config");
		// Iterate over the brewerys
		for (auto brewery_node = root_node->first_node("KeyBindings"); brewery_node; brewery_node = brewery_node->next_sibling("KeyBindings"))
		{
			for (auto beer_node = brewery_node->first_node("KeyBinding"); beer_node; beer_node = beer_node->next_sibling("KeyBinding"))
			{
				getInputManager()->setKeyBinding(keyBinding(beer_node->first_attribute("key")->value(), std::stoi(beer_node->first_attribute("value")->value())));
			}
		}

		for (auto brewery_node = root_node->first_node("Volume"); brewery_node; brewery_node = brewery_node->next_sibling("Volume"))
		{
			getSoundEngine()->setSoundVolume(std::stof(brewery_node->first_attribute("value")->value()));
		}

		theFile.close();
		doc.clear();
	}

	void Application::render()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		auto menus = getMenuManager()->getMenus();

		if (getMenuManager()->getLevelManager())
		{
			getMenuManager()->getLevelManager()->renderCurrentLevel(m_dt, getGameStateManager(), getInputManager(), getCollisionManager(), getRenderer(), getConfigurationManager(), getSpritesheetManager());
		}

		if (!menus->empty() && (getGameStateManager()->getGameState() == GameState::IN_MENU || getGameStateManager()->getGameState() == GameState::IN_PAUSED_MENU))
		{
			getMenuManager()->renderCurrentMenu(getRenderer(), m_dt, getConfigurationManager(), getInputManager());
		}

		glutSwapBuffers();
	}
}